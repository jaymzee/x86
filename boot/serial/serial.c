#include "system.h"
#define COM1_PORT 0x3f8      // COM1

void COM_Init() {
   outb(COM1_PORT + 1, 0x00);    // Disable all interrupts
   outb(COM1_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(COM1_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(COM1_PORT + 1, 0x00);    //                  (hi byte)
   outb(COM1_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(COM1_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(COM1_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
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

void COM_WriteString(const char *p)
{
    for (; *p; p++) {
        COM_WriteChar(*p);
    }
}

void Main()
{
    COM_Init();
    while (1) {
        COM_WriteString("press a key");
        char c = COM_ReadChar();
        COM_WriteString("you pressed: ");
        COM_WriteChar(c);
        COM_WriteString("\r\n");
    }
}
