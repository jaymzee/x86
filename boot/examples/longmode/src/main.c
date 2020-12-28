#include <conio.h>
#include <cpu.h>
#include <interrupt.h>
#include <serial.h>
#include <stdio.h>
#include <string.h>
#include "intsetup.h"
#include "isr.h"
#include "traps.h"

#include <system.h>

void ShowTimer(void);

void main()
{
    COM_Init();
    DisableBlinkingText(); // allows 16 background colors
    ClearText(0x07);
    TextCursorShape(13, 14); // underline
    fputs("long mode (x64) entered sucessfully!\n", console);
    fputs("connect to serial 0 (COM1) for the console\n", console);
    fputs("bang on the keyboard to show scan codes\n", console);
    println("long mode demo");
    EnableInterrupts();
    ShowTimer();
}

void CPUExceptionHandler(struct cpu_reg *reg, int except, int errcode)
{
    char regs[1000], msg[80], errstr[20];
    char *haltmsg = "system halted.\n";

    strcpy(msg, "\nPANIC: ");
    strcat(msg, cpu_exception[except]);
    strcat(msg, ", error code: ");
    strcat(msg, itoa(errcode, 16, 4, errstr));
    strcat(msg, "\n");
    DumpCPURegisters(regs, reg, 1);
    fputs(msg, console);
    fputs(regs, console);
    fputs(haltmsg, console);
    fputs(msg, stdout);
    fputs(regs, stdout);
    fputs(haltmsg, stdout);

    while (1) {
        __asm__ ("cli");
        __asm__ ("hlt");
    }
}

void ShowTimer(void)
{
    char buf[80];

    while (1) {
        print("press a key ");
        getchar();
        println("");
        print("timer_count: ");
        println(itoa(timer_count, 10, 0, buf));
    }
}
