#include <conio.h>
#include <serial.h>

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
    ClearScreen(0x1F);
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
