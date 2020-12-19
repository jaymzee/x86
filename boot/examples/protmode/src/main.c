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

extern int timer_count;
extern void load_idt(void *);
extern void keyboard_handler(void);

#define IDT_SIZE 256

struct IDT_descriptor IDT[IDT_SIZE];
struct IDT_info {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) IDT_info;

void InitializeIDT(void)
{
    for (int i = 0; i < 256; i++) {
        IDT[i].type_attr = 0;
        IDT[i].zero = 0;
        IDT[i].offset_hi = 0;
        IDT[i].offset_lo = 0;
        IDT[i].selector = 0;
    }

    IDT[0x21].offset_hi = (uint32_t)keyboard_handler >> 16;
    IDT[0x21].offset_lo = (uint32_t)keyboard_handler & 0xffff;
    IDT[0x21].selector = 0x08;
    IDT[0x21].type_attr = 0x8E;
    IDT[0x21].zero = 0;

    IDT_info.offset = (uint32_t)IDT;
    IDT_info.size = sizeof(IDT) - 1;

    load_idt(&IDT_info);    // enables cpu interrupts
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
    InitializeIDT();
    PIC_UnmaskIRQ(1);

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

    // write EOI
    outb(0x20, 0x20);

    status = inb(0x64);
    // Lowest bit of status will be set if buffer is not empty
    if (status & 0x01) {
        keycode = inb(0x60);
        if (keycode < 0)
            return;
        DisplayText(itoa(keycode, 16, buf));
    }
}
