#include "system.h"
#define PORT 0x3f8      // COM1

void InitSerial() {
   outb(PORT + 1, 0x00);    // Disable all interrupts
   outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(PORT + 1, 0x00);    //                  (hi byte)
   outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int IsTransmitEmpty()
{
    return inb(PORT + 5) & 0x20;
}
 
void WriteSerial(char a)
{
    while (!IsTransmitEmpty())
       ;

    outb(PORT, a);
}

int IsReceived()
{
    return inb(PORT + 5) & 1;
}
 
char ReadSerial()
{
    while (!IsReceived())
        ;

    return inb(PORT);
}

void Main()
{
    InitSerial();
    WriteSerial('H');
    WriteSerial('e');
    WriteSerial('l');
    WriteSerial('l');
    WriteSerial('o');
    WriteSerial('\r');
    WriteSerial('\n');
    WriteSerial('W');
    WriteSerial('o');
    WriteSerial('r');
    WriteSerial('l');
    WriteSerial('d');
}
