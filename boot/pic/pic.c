/*
 * intel 8259 interrupt controller
 */

#include "system.h"

#define PIC1        0x20
#define PIC2        0xA0
#define PIC1_CMD    PIC1
#define PIC1_DATA   (PIC1 + 1)
#define PIC2_CMD    PIC2
#define PIC2_DATA   (PIC2 + 1)

#define PIC_EOI     0x20        /* End-of-interrupt command code */

void PIC_sendEOI(unsigned char irq)
{
    if (irq >= 8) {
        outb(PIC2_CMD, PIC_EOI);
    }
    outb(PIC1_CMD, PIC_EOI);
}
