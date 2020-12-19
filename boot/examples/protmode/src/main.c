#include <conio.h>
#include <interrupt.h>
#include <serial.h>
#include <string.h>
#include <system.h>

#define IDT_ENTRIES 256
#define IDT_ADDRESS 0x1000
#define IDT_SIZE (IDT_ENTRIES * sizeof(struct IDT_entry))

extern int timer_count;
extern void load_idt(void *);
extern void keyboard_handler(void);
extern void timer_handler(void);
extern void divbyzero_handler(void);
extern void gpfault_handler(void);

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

// interrupts are disabled when main is called
// this sets up the interrupt descriptor tables then enables interrupts
void EnableInterrupts(void)
{
    struct IDT_entry *idt = (void *)IDT_ADDRESS;
    struct IDT_entry *descr;
    struct IDTR *idtr = (void *)(IDT_ADDRESS + IDT_SIZE);

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

    for (int i = 0; i < 256; i++) {
        idt[i].type_attr = 0;
        idt[i].zero = 0;
        idt[i].offset_hi = 0;
        idt[i].offset_lo = 0;
        idt[i].selector = 0;
    }

    descr = idt + 0;
    descr->offset_hi = (uint32_t)divbyzero_handler >> 16;
    descr->offset_lo = (uint32_t)divbyzero_handler & 0xffff;
    descr->selector = 0x08;
    descr->type_attr = 0x8F;
    descr->zero = 0;

    descr = idt + 0x0d;
    descr->offset_hi = (uint32_t)gpfault_handler >> 16;
    descr->offset_lo = (uint32_t)gpfault_handler & 0xffff;
    descr->selector = 0x08;
    descr->type_attr = 0x8F;
    descr->zero = 0;

    descr = idt + 0x20;
    descr->offset_hi = (uint32_t)timer_handler >> 16;
    descr->offset_lo = (uint32_t)timer_handler & 0xffff;
    descr->selector = 0x08;
    descr->type_attr = 0x8E;
    descr->zero = 0;

    descr = idt + 0x21;
    descr->offset_hi = (uint32_t)keyboard_handler >> 16;
    descr->offset_lo = (uint32_t)keyboard_handler & 0xffff;
    descr->selector = 0x08;
    descr->type_attr = 0x8E;
    descr->zero = 0;

    idtr->offset = (uint32_t)idt;
    idtr->limit = IDT_SIZE - 1;

    load_idt(idtr); // also enables cpu interrupts
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

    EnableInterrupts();
    PIC_UnmaskIRQ(1);   // keyboard
    PIC_UnmaskIRQ(0);   // timer

    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
        print("\n");
        print(itoa(timer_count, 10, buf));
    }
}

extern char kbd_decode[];

void keyboard_handler_main(void) {
    unsigned char status;
    int keycode;
    char buf[80];

    // write EOI
    outb(0x20, 0x20);

    status = inb(0x64);
    // Lowest bit of status will be set if buffer is not empty
    if (status & 0x01) {
        keycode = inb(0x60);
        if (keycode & 0x80)
            return;
        DisplayText(itoa(keycode, 16, buf));
        println(itoa(kbd_decode[keycode], 16, buf));
    }
}
