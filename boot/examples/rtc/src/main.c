#include "interrupt.h"
#include "serial.h"
#include "string.h"
#include "time.h"

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
    char buf[32];
    struct time now;

    COM_Init();
    print("Real time clock demo\n");
    while (1) {
        print("\ntick count: ");
        print(itoa(tick_counter, 10, 0, buf));
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
        switch (c) {
        case 'e':
            PIC_UnmaskIRQ(0);
            print("\nIRQ 0 enabled");
            break;
        case 'd':
            PIC_MaskIRQ(0);
            print("\nIRQ 0 disabled");
            break;
        case 't':
            RTC_GetTime(&now);
            print("\nTime is ");
            print(itoa(now.hour, 10, 2, buf));
            print(":");
            print(itoa(now.minute, 10, 2, buf));
            print(":");
            print(itoa(now.second, 10, 2, buf));
        }
    }
}
