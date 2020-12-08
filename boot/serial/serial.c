/*
 * 8250 original IBM PC UART (limited to 9600 bps)
 * 16450 same arch but faster bps (dos okay at 9600 but not a multitasking OS)
 * 16550 has a 16 byte FIFO
 * 16750 has a 64 byte FIFO
 */

#include "system.h"
#define COM1_PORT 0x3f8      // COM1

void COM_Init() {
   outb(COM1_PORT + 1, 0x00);   // Disable all interrupts
   outb(COM1_PORT + 3, 0x80);   // Enable DLAB (set baud rate divisor)
   outb(COM1_PORT + 0, 12);     // Set divisor to 12 (lo byte) 9600 baud
   outb(COM1_PORT + 1, 0);      //                   (hi byte)
   outb(COM1_PORT + 3, 0x03);   // 8 bits, no parity, one stop bit
   outb(COM1_PORT + 2, 0xC7);   // Enable FIFO, clear them, with 14-byte thresh
   outb(COM1_PORT + 4, 0x0B);   // AuxOut2 (IRQ) enabled, RTS/DTR set
}

// is serial transmitter empty
int COM_IsTxEmpty()
{
    return inb(COM1_PORT + 5) & 0x20;
}

// blocking
void COM_WriteChar(char a)
{
    while (!COM_IsTxEmpty())
       ;

    outb(COM1_PORT, a);
}

// does serial receive have data
int COM_IsRxReady()
{
    return inb(COM1_PORT + 5) & 1;
}

// blocking
char COM_ReadChar()
{
    while (!COM_IsRxReady())
        ;

    return inb(COM1_PORT);
}

void COM_WriteString(const char *str)
{
    for (char ch = *str; ch; ch = *++str) {
        if (ch == 10) {
            COM_WriteChar(13);
        }
        COM_WriteChar(ch);
    }
}

void Main()
{
    COM_Init();
    COM_WriteString("Welcome to my first bootloader\n");
    COM_WriteString("Stay tuned for enabling protected mode\n");
    while (1) {
        COM_WriteString("\npress a key ");
        char c = COM_ReadChar();
        COM_WriteString("\nyou pressed: ");
        COM_WriteChar(c);
    }
}
