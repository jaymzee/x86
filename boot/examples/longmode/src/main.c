#include "conio.h"
#include "interrupt.h"
#include "serial.h"
#include "string.h"

#define TXT_ATTR    0x07

void print(const char *str)
{
    COM_WriteString(str);
}

void println(const char *str)
{
    COM_WriteString(str);
    COM_WriteString("\n");
}

int getchar(void)
{
    return COM_ReadChar();
}

void main()
{
    long *p = (long *)0x2000;
    char buf[80];
    int n = 0;

    DisplayText("long mode (x64) entered sucessfully!", TXT_ATTR);
    DisplayText("Initializing serial 0...", TXT_ATTR);
    COM_Init();
    DisplayText("connect to serial 0 (COM1) for the console", TXT_ATTR);
    println("long mode demo");
    println("page tables:");
    print("PML4T[0] = 0x");
    println(itoa(p[0], 16, buf));
    print("PDPT[0] = 0x");
    println(itoa(p[0x200], 16, buf));
    print("PDT[0] = 0x");
    println(itoa(p[0x400], 16, buf));
    for (int i = 0; i < 16; i++, n++) {
        print("PT[");
        print(itoa(n, 10, buf));
        print("] = ");
        println(itoa(p[0x600 + n], 16, buf));
    }

    while (1) {
        print("press a key ");
        getchar();
        print("\n");
        for (int i = 0; i < 16; i++, n++) {
            if (n < 1024) {
                print("PT[");
                print(itoa(n, 10, buf));
                print("] = ");
                println(itoa(p[0x600 + n], 16, buf));
            }
        }
    }
}

void main() __attribute__((section(".text.main")));

