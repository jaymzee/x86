#include <conio.h>
#include <cpu.h>
#include <serial.h>
#include <stdio.h>
#include <string.h>
#include "intsetup.h"

extern void CauseGPFault(void);
extern void CauseDivByZeroFault(void);

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
        print(itoa(timer_count, 10, 0, buf));
    }
}

void GPFaultHandlerM(int errcode, const struct cpu_reg *reg)
{
    char mesg[80], selector[20];

    strcpy(mesg, "PANIC: GP Fault, selector: ");
    strcat(mesg, itoa(errcode, 16, 4, selector));

    DisplayText(mesg); // print to screen
    DumpCPURegisters(reg, 1); // dump to screen

    print("\n");
    println(mesg); // print to serial 0
    DumpCPURegisters(reg, 0); // dump to serial 0
}

void DivByZeroHandlerM(const struct cpu_reg *reg)
{
    const char *mesg = "PANIC: div by zero";

    DisplayText(mesg); // print to screen
    DumpCPURegisters(reg, 1); // dump to screen

    print("\n");
    println(mesg); // print to serial 0
    DumpCPURegisters(reg, 0); // dump to serial 0
}

void KeyboardHandlerM(void) {
    int keycode = ScanKeyboard();
    // Lowest bit of status will be set if buffer is not empty
    if (keycode >= 0 && keycode < 128) {
        char buf[80];
        char ch = kbd_decode[keycode];
        strcpy(buf, "0x");
        itoa(keycode, 16, 2, buf+2);
        strcat(buf, "  ");
        int len = strlen(buf);
        buf[len - 1] = ch;
        DisplayText(buf);
    }
}
