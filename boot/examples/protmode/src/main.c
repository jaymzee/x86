#include <conio.h>
#include <serial.h>
#include <stdio.h>
#include <string.h>
#include "intsetup.h"

void main()
{
    char buf[80];

    DisableBlink();
    ClearScreen(0x1F);
    DisplayText("32-bit protected mode entered successfully!");
    DisplayText("connect to serial 0 (COM1) for the console");
    DisplayText("bang on keyboard to show scan codes");

    COM_Init();

    println("installing handlers and enabling interrupts...");
    EnableInterrupts();
    println("32-bit protected mode demo");

    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
        print("\n");
        print(itoa(timer_count, 10, buf));
    }
}

