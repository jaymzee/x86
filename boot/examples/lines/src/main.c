#include <graphics.h>
#include <serial.h>
#include <stdio.h>

void main()
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
