/*
 * intel 8259 interrupt controller
 */

#include "interrupt.h"
#include "system.h"

#define PIC1            0x20        // PIC controller 1 base address
#define PIC2            0xA0        // PIC controller 2 base address
#define PIC1_CMD        PIC1        // PIC controller 1 command port
#define PIC1_DATA       (PIC1 + 1)  // PIC controller 1 data port
#define PIC2_CMD        PIC2        // PIC controller 2 command port
#define PIC2_DATA       (PIC2 + 1)  // PIC controller 2 data port

#define OCW2_EOI        0x20    // OCW2 End-of-interrupt command code
#define OCW3_READ_IRR   0x0a    // OCW3 irq ready next CMD read
#define OCW3_READ_ISR   0x0b    // OCW3 irq service next CMD read

#define ICW1_ICW4       0x01    // ICW4 (not) needed
#define ICW1_SINGLE     0x02    // Single (cascade) mode
#define ICW1_INTERVAL4  0x04    // Call address interval 4 (8)
#define ICW1_LEVEL      0x08    // Level triggered (edge) mode
#define ICW1_INIT       0x10    // Initialization - required!

#define ICW4_8086       0x01    // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO       0x02    // Auto (normal) EOI
#define ICW4_BUF_SLAVE  0x08    // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C    // Buffered mode/master
#define ICW4_SFNM       0x10    // Special fully nested (not)

#define RTC             0x70    // CMOS/Real Time Clock/disable NMI

// on older machines its necessary to give the PIC some time to react
// to commands as they might not be processed quickly
#define wait() __asm__("nop")

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default
    offset1 - vector offset for master PIC
              vectors on the master become offset1..offset1+7
    offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_RemapIVT(int offset1, int offset2)
{
    unsigned char a1, a2;

    a1 = inb(PIC1_DATA);        // save masks
    a2 = inb(PIC2_DATA);

    // start the initialization sequence (in cascade mode)
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    wait();
    outb(PIC1_DATA, offset1);   // ICW2: Master PIC vector offset
    wait();
    outb(PIC2_DATA, offset2);   // ICW2: Slave PIC vector offset
    wait();
    outb(PIC1_DATA, 4);         // ICW3: tell Master PIC that there is a slave
    wait();                     //       PIC at IRQ2 (0000 0100)
    outb(PIC2_DATA, 2);         // ICW3: tell Slave PIC its cascade identity
    wait();                     //       (0000 0010)
    outb(PIC1_DATA, ICW4_8086);
    wait();
    outb(PIC2_DATA, ICW4_8086);
    wait();

    outb(PIC1_DATA, a1);        // restore saved masks.
    outb(PIC2_DATA, a2);
}

// issue non-specific EOI command using OCW2
void PIC_SendEOI(unsigned char irq)
{
    if (irq >= 8) {
        outb(PIC2_CMD, OCW2_EOI);
    }
    outb(PIC1_CMD, OCW2_EOI);
}

// mask IRQ bit using OCW1
void PIC_MaskIRQ(unsigned char irq) {
    unsigned short port;
    unsigned char value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);
}

// unmask IRQ bit using OCW1
void PIC_UnmaskIRQ(unsigned char irq) {
    unsigned short port;
    unsigned char value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

// helper function
static unsigned short PIC_IssueOCW3(unsigned char ocw3)
{
    // OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
    // represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain
    outb(PIC1_CMD, ocw3);
    outb(PIC2_CMD, ocw3);
    return (inb(PIC2_CMD) << 8) | inb(PIC1_CMD);
}

// Returns the combined value of the cascaded PICs irq request register
unsigned short PIC_GetIRR(void)
{
    return PIC_IssueOCW3(OCW3_READ_IRR);
}

// Returns the combined value of the cascaded PICs in service register
unsigned short PIC_GetISR(void)
{
    return PIC_IssueOCW3(OCW3_READ_ISR);
}

// unmask NMI via CMOS/RTC
void NMI_Enable()
{
    outb(RTC, inb(RTC) & 0x7F);
}

// mask NMI via CMOS/RTC
void NMI_Disable()
{
     outb(RTC, inb(RTC) | 0x80);
}
