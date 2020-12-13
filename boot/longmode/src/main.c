#include "interrupt.h"
#include "serial.h"
#include "string.h"

static void print(const char *str)
{
    COM_WriteString(str);
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
    print("protected mode demo\n");
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
