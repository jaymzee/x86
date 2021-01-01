#include <stdio.h>
#include <string.h>
#include <sys/conio.h>
#include <sys/interrupt.h>
#include <sys/serial.h>
#include <sys/timer.h>
#include "isr.h"

#define CS 0x08
#define DPL 0

void InstallISRs(void)
{
    struct IDT_entry *idt = (void *)IDT_ADDRESS;
    struct IDTR *idtr = (void *)(IDTR_ADDRESS);

    IDT_IntGate(idt + 0x21, KeyboardHandler, CS, DPL);
    IDT_IntGate(idt + 0x20, TimerHandler, CS, DPL);
    idtr->limit = 0x22 * sizeof(struct IDT_entry) - 1;

    LoadIDT(idtr); // also enables cpu interrupts

    PIC_UnmaskIRQ(1); // enable keyboard IRQ
    PIC_UnmaskIRQ(0); // enable timer IRQ
}

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
    InstallISRs();
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

