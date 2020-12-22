#include <conio.h>
#include <interrupt.h>
#include <serial.h>
#include <stdio.h>
#include <string.h>
#include <x86_64.h>
#include "intsetup.h"

void CrashMe(void);

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
    ClearScreen(0x1F);
    DisplayText("long mode (x64) entered sucessfully!");
    DisplayText("connect to serial 0 (COM1) for the console");
    println("long mode demo");

    EnableInterrupts();
    CrashMe();

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

void DumpError(const char *exception, int errcode, struct x86_64_regs *r)
{
    char sbuf[128];
    char nbuf[20];

    DisplayText("");
    strcpy(sbuf, "PANIC: CPU Exception: ");
    strcat(sbuf, exception);
    strcat(sbuf, ", Error Code: ");
    strcat(sbuf, itoa(errcode, 16, 4, nbuf));
    DisplayText(sbuf);

    strcpy(sbuf, "  rip: ");
    strcat(sbuf, itoa(r->rip, 16, 16, nbuf));
    strcat(sbuf, "     rsp: ");
    strcat(sbuf, itoa(r->rsp, 16, 16, nbuf));
    strcat(sbuf, "   flags: ");
    strcat(sbuf, itoa(r->rflags, 16, 16, nbuf));
    DisplayText(sbuf);

    strcpy(sbuf, "  rax: ");
    strcat(sbuf, itoa(r->rax, 16, 16, nbuf));
    strcat(sbuf, "     rbx: ");
    strcat(sbuf, itoa(r->rbx, 16, 16, nbuf));
    strcat(sbuf, "     rcx: ");
    strcat(sbuf, itoa(r->rcx, 16, 16, nbuf));
    DisplayText(sbuf);

    strcpy(sbuf, "  rdx: ");
    strcat(sbuf, itoa(r->rdx, 16, 16, nbuf));
    strcat(sbuf, "     rsi: ");
    strcat(sbuf, itoa(r->rsi, 16, 16, nbuf));
    strcat(sbuf, "     rdi: ");
    strcat(sbuf, itoa(r->rdi, 16, 16, nbuf));
    DisplayText(sbuf);

    strcpy(sbuf, "  rbp: ");
    strcat(sbuf, itoa(r->rbp, 16, 16, nbuf));
    strcat(sbuf, "      r8: ");
    strcat(sbuf, itoa(r->r8, 16, 16, nbuf));
    strcat(sbuf, "      r9: ");
    strcat(sbuf, itoa(r->r9, 16, 16, nbuf));
    DisplayText(sbuf);

    strcpy(sbuf, "  r10: ");
    strcat(sbuf, itoa(r->r10, 16, 16, nbuf));
    strcat(sbuf, "     r11: ");
    strcat(sbuf, itoa(r->r11, 16, 16, nbuf));
    strcat(sbuf, "     r12: ");
    strcat(sbuf, itoa(r->r12, 16, 16, nbuf));
    DisplayText(sbuf);

    strcpy(sbuf, "  r13: ");
    strcat(sbuf, itoa(r->r13, 16, 16, nbuf));
    strcat(sbuf, "     r14: ");
    strcat(sbuf, itoa(r->r14, 16, 16, nbuf));
    strcat(sbuf, "     r15: ");
    strcat(sbuf, itoa(r->r15, 16, 16, nbuf));
    DisplayText(sbuf);

    strcpy(sbuf, "  cs: ");
    strcat(sbuf, itoa(r->cs, 16, 4, nbuf));
    strcat(sbuf, "  ss: ");
    strcat(sbuf, itoa(r->ss, 16, 4, nbuf));
    strcat(sbuf, "  ds: ");
    strcat(sbuf, itoa(r->ds, 16, 4, nbuf));
    strcat(sbuf, "  es: ");
    strcat(sbuf, itoa(r->es, 16, 4, nbuf));
    strcat(sbuf, "  fs: ");
    strcat(sbuf, itoa(r->fs, 16, 4, nbuf));
    strcat(sbuf, "  gs: ");
    strcat(sbuf, itoa(r->gs, 16, 4, nbuf));
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
