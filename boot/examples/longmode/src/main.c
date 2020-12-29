#include <conio.h>
#include <cpu.h>
#include <interrupt.h>
#include <serial.h>
#include <stdio.h>
#include <string.h>
#include <timer.h>
#include "intsetup.h"
#include "isr.h"
#include "traps.h"

#define STACK_TOP 0x1000000 // top of 16MB of memory

void ShowTimer(void);

void main()
{
    COM_Init();
    SetIntervalTimer(60);
    EnableInterrupts();
    DisableBlinkingText(); // allows 16 background colors
    ClearText(0x07);
    TextCursorShape(13, 14); // underline
    fputs("long mode (x64) entered sucessfully!\n", console);
    fputs("connect to serial 0 (COM1) for the console\n", console);
    fputs("bang on the keyboard to show scan codes\n", console);
    println("long mode demo");
    ShowTimer();
}

void DumpStack(char *sbuf, uint64_t *stkptr)
{
    char tmpstr[17];
    strcpy(sbuf, "Stack:");
    for (int i = 0; i < 12 && (long)stkptr < (STACK_TOP - 7); i++, stkptr++) {
        if ((i % 3) == 0) {
            strcat(sbuf, "\n  ");
            strcat(sbuf, ltoa((long)stkptr, 16, 12, tmpstr));
            strcat(sbuf, ":");
        }
        strcat(sbuf, " ");
        strcat(sbuf, ltoa(*stkptr, 16, 16, tmpstr));
    }
    strcat(sbuf, "\n");
}

void CPUExceptionHandler(struct cpu_reg *reg, int except, int errcode)
{
    char regs[768], stack[384], msg[81], tmpstr[17];
    char *haltmsg = "system halted.\n";

    strcpy(msg, "\nPANIC: ");
    strcat(msg, cpu_exception[except]);
    strcat(msg, ", error code: ");
    strcat(msg, itoa(errcode, 16, 4, tmpstr));
    strcat(msg, "\n");
    DumpCPURegisters(regs, reg, 1);
    DumpStack(stack, (uint64_t *)reg->rsp);
    fputs(msg, console);
    fputs(regs, console);
    fputs(stack, console);
    fputs(haltmsg, console);
    fputs(msg, stdout);
    fputs(regs, stdout);
    fputs(stack, stdout);
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
