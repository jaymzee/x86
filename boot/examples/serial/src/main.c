#include "serial.h"

static void print(const char *str)
{
    COM_WriteString(str);
}

static void println(const char *str)
{
    COM_WriteString(str);
    COM_WriteString("\n");
}

static int getchar(void)
{
    return COM_ReadChar();
}

static int putchar(int ch)
{
    COM_WriteChar(ch);
    return ch;
}

void Main(void)
{
    COM_Init();
    print("Welcome to my first bootloader\n");
    print("Stay tuned for enabling protected mode\n");
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
