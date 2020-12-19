#include <conio.h>
#include <interrupt.h>
#include <serial.h>
#include <string.h>
#include <system.h>

void print(const char *str)
{
    COM_WriteString(str);
}

void println(const char *str)
{
    COM_WriteString(str);
    COM_WriteString("\n");
}

int getchar(void)
{
    return COM_ReadChar();
}

int putchar(int ch)
{
    COM_WriteChar(ch);
    return ch;
}

/*
struct IDT_descriptor IDT[256];
struct IDT_info {
    uint16_t size;
    uint32_t offset;
} IDT_info;

extern void load_idt(struct IDT_info *);
extern void kb_isr(void);
extern void timer_isr(void);
extern void gp_fault(void);
extern void divbyzero_fault(void);
extern int timer_count;

void idt_init1(void)
{
    for (int i = 0; i < 256; i++) {
        IDT[i].type_attr = 0;
        IDT[i].zero = 0;
        IDT[i].offset_hi = 0;
        IDT[i].offset_lo = 0;
        IDT[i].selector = 0;
    }

    IDT[0x00].offset_hi = (uint32_t)divbyzero_fault >> 16;
    IDT[0x00].offset_lo = (uint32_t)divbyzero_fault & 0xffff;
    IDT[0x00].selector = 0x08;
    IDT[0x00].type_attr = 0x8F;
    IDT[0x00].zero = 0;

    IDT[0x0d].offset_hi = (uint32_t)gp_fault >> 16;
    IDT[0x0d].offset_lo = (uint32_t)gp_fault & 0xffff;
    IDT[0x0d].selector = 0x08;
    IDT[0x0d].type_attr = 0x8F;
    IDT[0x0d].zero = 0;

    IDT[0x20].offset_hi = (uint32_t)timer_isr >> 16;
    IDT[0x20].offset_lo = (uint32_t)timer_isr & 0xffff;
    IDT[0x20].selector = 0x08;
    IDT[0x20].type_attr = 0x8E;
    IDT[0x20].zero = 0;

    IDT[0x20].offset_hi = (uint32_t)kb_isr >> 16;
    IDT[0x20].offset_lo = (uint32_t)kb_isr & 0xffff;
    IDT[0x20].selector = 0x08;
    IDT[0x20].type_attr = 0x8E;
    IDT[0x20].zero = 0;

    IDT_info.offset = (uint32_t)IDT;
    IDT_info.size = sizeof(IDT) - 1;

    load_idt(&IDT_info);
}

*/

extern int timer_count;
extern void load_idt(void *);

extern void keyboard_handler(void);

struct IDT_entry{
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

#define IDT_SIZE 256
struct IDT_entry IDT[IDT_SIZE];

void idt_init2(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
	

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	outb(0x20 , 0x11);
	outb(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	outb(0x21 , 0x20);
	outb(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	outb(0x21 , 0x00);  
	outb(0xA1 , 0x00);  

	/* ICW4 - environment info */
	outb(0x21 , 0x01);
	outb(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	outb(0x21 , 0xff);
	outb(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}

void main()
{
    char buf[80];
    DisableBlink();
    ClearScreen(0x1F);
    DisplayText("32-bit protected mode entered successfully!");
    DisplayText("initializing serial port 0...");
    COM_Init();
    DisplayText("connect to serial 0 (COM1) for the console");
    println("32-bit protected mode demo");

    /*
    PIC_RemapIVT(0x20, 0x28);
    PIC_MaskIRQ(0);
    PIC_MaskIRQ(1);
    PIC_MaskIRQ(3);
    PIC_MaskIRQ(4);
    PIC_MaskIRQ(5);
    PIC_MaskIRQ(6);
    PIC_MaskIRQ(7);
    PIC_MaskIRQ(8);
    PIC_MaskIRQ(12);
    PIC_MaskIRQ(13);
    PIC_MaskIRQ(14);
    PIC_MaskIRQ(15);
    */

    //load_idt(&IDT_info);

    //PIC_UnmaskIRQ(1);
    //
    idt_init2();

    outb(0x21, 0xFD); // enable IRQ1

    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
        print("\n");
        print(itoa(timer_count, 10, buf));
    }
}

void keyboard_handler_main(void) {
	unsigned char status;
	char keycode;
        char buf[80];

	/* write EOI */
	outb(0x20, 0x20);

	status = inb(0x64);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = inb(0x60);
		if(keycode < 0)
			return;
		DisplayText(itoa(keycode, 16, buf));
	}
}
