#include <stdio.h>
#include <string.h>
#include <sys/conio.h>
#include <sys/serial.h>
#include <sys/timer.h>
#include "intsetup.h"
#include "isr.h"
#include "traps.h"

void ShowTimer(void)
{
    char buf[80];

    while (1) {
        print("press a key ");
        int ch = getchar();
        println("");
        print("timer_count: ");
        println(itoa(timer_count, 10, 0, buf));
        if (ch == 'b')
            Beep(1000);
        if (ch == 's')
            NoSound();
    }
}

int main(int argc, char *argv[], char *envp[])
{
    EnableInterrupts();
    SetIntervalTimer(60);
    DisableBlinkingText(); // allows 16 background colors
    TextCursorShape(13, 14); // underline
    ClearText(0x07);
    fputs("long mode (x64) entered sucessfully!\n", console);
    fputs("connect to serial 0 (COM1) for the console\n", console);
    fputs("bang on the keyboard to show scan codes\n", console);
    println("long mode demo");
    ShowTimer();
    return 0;
}
