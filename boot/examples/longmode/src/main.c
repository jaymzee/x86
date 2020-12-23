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
    ClearScreen(0x07);
    DisplayText("long mode (x64) entered sucessfully!");
    DisplayText("connect to serial 0 (COM1) for the console");
    println("long mode demo");

    EnableInterrupts();

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
        print("\n");
        for (int i = 0; i < 16; i++, n++) {
            if (n < 4096) {
                print("PT[");
                print(itoa(n, 10, 0, buf));
                print("] = 0x");
                println(itoa(pt[n], 16, 8, buf));
            }
        }
        println(itoa(timer_count, 10, 0, buf));
    }
}

void GPFaultHandlerM(int errcode, struct cpu_reg *reg)
{
    char mesg[80], selector[20];

    strcpy(mesg, "PANIC: GP Fault, selector: ");
    strcat(mesg, itoa(errcode, 16, 4, selector));

    DisplayText(mesg); // print to screen
    DumpCPURegisters(reg, 1, 1); // dump to screen

    print("\n");
    println(mesg); // print to serial 0
    DumpCPURegisters(reg, 0, 1); // dump to serial 0
}

void DivbyzeroHandlerM(struct cpu_reg *reg)
{
    const char *mesg = "PANIC: divide by zero";

    DisplayText(mesg); // print to screen
    DumpCPURegisters(reg, 1, 1); // dump to screen

    print("\n");
    println(mesg); // print to serial 0
    DumpCPURegisters(reg, 0, 1); // dump to serial 0
}

void PageFaultHandlerM(int errcode, struct cpu_reg *reg)
{
    char mesg[80], errstr[20];

    strcpy(mesg, "PANIC: Page Fault, error code: ");
    strcat(mesg, itoa(errcode, 16, 4, errstr));
    strcat(mesg, " IRUWP");

    DisplayText(mesg); // print to screen
    DumpCPURegisters(reg, 1, 1); // dump to screen

    print("\n");
    println(mesg); // print to serial 0
    DumpCPURegisters(reg, 0, 1); // dump to serial 0
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
