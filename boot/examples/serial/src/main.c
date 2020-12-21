#include <serial.h>
#include <stdio.h>

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
