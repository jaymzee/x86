#include <interrupt.h>
#include <serial.h>
#include <string.h>
#include <graphics.h>

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
    for (int i = 0; i < 200; i++) {
        DrawLine(0, i, 319, i, i);
    }
    COM_Init();
    println("protected mode demo");
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
