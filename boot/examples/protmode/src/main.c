#include <conio.h>
#include <cpu.h>
#include <serial.h>
#include <stdio.h>
#include <string.h>
#include "intsetup.h"

extern void CrashMe(void);

void main()
{
    char buf[80];
    struct cpu_reg reg;

    DisableBlink();
    ClearScreen(0x1F);
    DisplayText("32-bit protected mode entered successfully!");
    DisplayText("connect to serial 0 (COM1) for the console");
    DisplayText("bang on keyboard to show scan codes");

    COM_Init();

    DumpCPURegisters(&reg, 0);
    DumpCPURegisters(&reg, 1);

    println("installing handlers and enabling interrupts...");
    EnableInterrupts();
    println("32-bit protected mode demo");
    CrashMe();
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
        print("\n");
        print(itoa(timer_count, 10, 0, buf));
    }
}

void DumpException(const char *type, int error, int eip, int cs, int eflags)
{
    char sbuf[128];
    char nbuf[20];

    strcpy(sbuf, type);
    strcat(sbuf, ": Error Code=");
    itoa(error, 16, 4, nbuf);
    strcat(sbuf, nbuf);
    strcat(sbuf, " EIP=");
    itoa(cs, 16, 4, nbuf);
    strcat(sbuf, nbuf);
    strcat(sbuf, ":");
    itoa(eip, 16, 8, nbuf);
    strcat(sbuf, nbuf);
    strcat(sbuf, " EFLAGS=");
    itoa(eflags, 16, 8, nbuf);
    strcat(sbuf, nbuf);
    DisplayText(sbuf);
}

void KeyboardHandlerMain(void) {
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
