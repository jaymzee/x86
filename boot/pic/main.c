#include "serial.h"
#include "util.h"

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
    extern int tick_counter;
    char buf[64];
    COM_Init();
    print("Welcome to my first bootloader\n");
    print("Stay tuned for enabling protected mode\n");
    while (1) {
        print("\ntick count: ");
        print(itoa(tick_counter, 10, buf));
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
