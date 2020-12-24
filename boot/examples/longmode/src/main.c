#include <conio.h>
#include <cpu.h>
#include <interrupt.h>
#include <serial.h>
#include <stdio.h>
#include <string.h>
#include "intsetup.h"

void CauseGPFault(void);
void CausePageFault(void);
void CauseDivbyzero(void);

void main()
{
    long *pt = (long *)0x8000;
    long *pdt = (long *)0x6000;
    long *pdpt = (long *)0x5000;
    long *pml4t = (long *)0x4000;
    char buf[80];
    int n = 0;

    COM_Init();
    DisableBlink();
    ClearText(0x1F);
    fputs("long mode (x64) entered sucessfully!\n", console);
    fputs("connect to serial 0 (COM1) for the console\n", console);

    EnableInterrupts();

    println("long mode demo");
    println("page tables:");
    print("PML4T[0] = 0x");
    println(itoa(pml4t[0], 16, 8, buf));
    print("PDPT[0] = 0x");
    println(itoa(pdpt[0], 16, 8, buf));
    for (int i = 0; i < 2; i++) {
        print("PDT[");
        print(itoa(i, 10, 0, buf));
        print("] = 0x");
        println(itoa(pdt[i], 16, 8, buf));
    }
    for (int i = 0; i < 8; i++, n++) {
        print("PT[");
        print(itoa(n, 10, 0, buf));
        print("] = 0x");
        println(itoa(pt[n], 16, 8, buf));
    }
    while (1) {
        print("press a key ");
        getchar();
        println("");
        for (int i = 0; i < 16; i++, n++) {
            if (n < 4096) {
                print("PT[");
                print(itoa(n, 10, 0, buf));
                print("] = 0x");
                println(itoa(pt[n], 16, 8, buf));
            }
        }
        print("timer_count: ");
        println(itoa(timer_count, 10, 0, buf));
    }
}

void CPUException(struct cpu_reg *reg, int exception, int errcode)
{
    char regs[1000], msg[80], ecs[20];

    strcpy(msg, "\nPANIC: ");
    strcat(msg, cpu_exceptions[exception]);
    strcat(msg, ", error code: ");
    strcat(msg, itoa(errcode, 16, 4, ecs));
    strcat(msg, "\n");
    DumpCPURegisters(regs, reg, 1);
    fputs(msg, console);
    fputs(regs, console);
    fputs(msg, stdout);
    fputs(regs, stdout);
}

void KeyboardHandlerM(void) {
    int keycode = ScanKeyboard();
    // Lowest bit of status will be set if buffer is not empty
    if (keycode >= 0 && keycode < 128) {
        char buf[80];
        char ch = kbd_decode[keycode];
        strcpy(buf, "0x");
        itoa(keycode, 16, 2, buf+2);
        strcat(buf, "  \n");
        int len = strlen(buf);
        if (ch > 31 && ch < 127) {
            buf[len - 2] = ch;
        }
        fputs(buf, console);
    }
}
