#include <interrupt.h>
#include <serial.h>
#include <string.h>

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

void Main()
{
    COM_Init();
    println("protected mode demo");
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
