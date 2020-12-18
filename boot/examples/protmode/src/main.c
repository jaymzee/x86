#include <conio.h>
#include <interrupt.h>
#include <serial.h>
#include <string.h>
#include <system.h>
#include <vga.h>

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

void main()
{
    DisableBlink();
    ClearScreen(0xF1);
    DisplayText("32-bit protected mode entered successfully!");
    DisplayText("initializing serial port 0...");
    COM_Init();
    DisplayText("connect to serial 0 (COM1) for the console");
    println("32-bit protected mode demo");
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}

void main() __attribute__((section(".text.main")));
