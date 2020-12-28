#include <graphics.h>
#include <serial.h>
#include <stdio.h>

#include <vga.h>
#include <system.h>

void set_palette()
{
    for (int i = 0; i < 64; i++) {
        outb(VGA_DAC_ADDR_WR, i);
        outb(VGA_DAC_DATA, i);
        outb(VGA_DAC_DATA, i);
        outb(VGA_DAC_DATA, i);
    }
    for (int i = 64; i < 128; i++) {
        outb(VGA_DAC_ADDR_WR, i);
        outb(VGA_DAC_DATA, i);
        outb(VGA_DAC_DATA, 0);
        outb(VGA_DAC_DATA, 0);
    }
    for (int i = 128; i < 196; i++) {
        outb(VGA_DAC_ADDR_WR, i);
        outb(VGA_DAC_DATA, 0);
        outb(VGA_DAC_DATA, i);
        outb(VGA_DAC_DATA, 0);
    }
    for (int i = 196; i < 256; i++) {
        outb(VGA_DAC_ADDR_WR, i);
        outb(VGA_DAC_DATA, 0);
        outb(VGA_DAC_DATA, 0);
        outb(VGA_DAC_DATA, i);
    }
}

void main()
{
    set_palette();
    for (int i = 0; i < 256; i++) {
        DrawLine(i, 0, i, 199, i);
    }
    COM_Init();
    println("VGA graphics in long mode (64-bit) demo");
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
