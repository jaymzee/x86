#include <conio.h>
#include <cpu.h>
#include <stdio.h>
#include <string.h>

#if __x86_64__
// DumpCPURegister dumps the CPU registers to the screen or serial port
//   dest = 1 for screen
//   dest = 0 for serial port 0 (COM1)
//   ctrl = 1 to show control registers
void DumpCPURegisters(const struct cpu_reg *reg, int dest, int ctrl)
{
    char sbuf[80];
    char nbuf[20];
    void (*dump)(const char *);

    dump = println;
    if (dest) {
        dump = DisplayText;
    }

    strcpy(sbuf, "  rip: ");
    strcat(sbuf, itoa(reg->rip, 16, 16, nbuf));
    strcat(sbuf, "     rsp: ");
    strcat(sbuf, itoa(reg->rsp, 16, 16, nbuf));
    strcat(sbuf, "   flags: ");
    strcat(sbuf, itoa(reg->rflags, 16, 16, nbuf));
    dump(sbuf);

    strcpy(sbuf, "  rax: ");
    strcat(sbuf, itoa(reg->rax, 16, 16, nbuf));
    strcat(sbuf, "     rbx: ");
    strcat(sbuf, itoa(reg->rbx, 16, 16, nbuf));
    strcat(sbuf, "     rcx: ");
    strcat(sbuf, itoa(reg->rcx, 16, 16, nbuf));
    dump(sbuf);

    strcpy(sbuf, "  rdx: ");
    strcat(sbuf, itoa(reg->rdx, 16, 16, nbuf));
    strcat(sbuf, "     rsi: ");
    strcat(sbuf, itoa(reg->rsi, 16, 16, nbuf));
    strcat(sbuf, "     rdi: ");
    strcat(sbuf, itoa(reg->rdi, 16, 16, nbuf));
    dump(sbuf);

    strcpy(sbuf, "  rbp: ");
    strcat(sbuf, itoa(reg->rbp, 16, 16, nbuf));
    strcat(sbuf, "      r8: ");
    strcat(sbuf, itoa(reg->r8, 16, 16, nbuf));
    strcat(sbuf, "      r9: ");
    strcat(sbuf, itoa(reg->r9, 16, 16, nbuf));
    dump(sbuf);

    strcpy(sbuf, "  r10: ");
    strcat(sbuf, itoa(reg->r10, 16, 16, nbuf));
    strcat(sbuf, "     r11: ");
    strcat(sbuf, itoa(reg->r11, 16, 16, nbuf));
    strcat(sbuf, "     r12: ");
    strcat(sbuf, itoa(reg->r12, 16, 16, nbuf));
    dump(sbuf);

    strcpy(sbuf, "  r13: ");
    strcat(sbuf, itoa(reg->r13, 16, 16, nbuf));
    strcat(sbuf, "     r14: ");
    strcat(sbuf, itoa(reg->r14, 16, 16, nbuf));
    strcat(sbuf, "     r15: ");
    strcat(sbuf, itoa(reg->r15, 16, 16, nbuf));
    dump(sbuf);

    if (ctrl) {
        strcpy(sbuf, "  cr0: ");
        strcat(sbuf, itoa(reg->cr0, 16, 16, nbuf));
        strcat(sbuf, "     cr2: ");
        strcat(sbuf, itoa(reg->cr2, 16, 16, nbuf));
        strcat(sbuf, "     cr3: ");
        strcat(sbuf, itoa(reg->cr3, 16, 16, nbuf));
        dump(sbuf);

        strcpy(sbuf, "  cr4: ");
        strcat(sbuf, itoa(reg->cr4, 16, 16, nbuf));
        strcat(sbuf, "     cr8: ");
        strcat(sbuf, itoa(reg->cr8, 16, 16, nbuf));
        dump(sbuf);
    }

    strcpy(sbuf, "   cs: ");
    strcat(sbuf, itoa(reg->cs, 16, 4, nbuf));
    strcat(sbuf, "    ss: ");
    strcat(sbuf, itoa(reg->ss, 16, 4, nbuf));
    strcat(sbuf, "      ds: ");
    strcat(sbuf, itoa(reg->ds, 16, 4, nbuf));
    strcat(sbuf, "    es: ");
    strcat(sbuf, itoa(reg->es, 16, 4, nbuf));
    strcat(sbuf, "      fs: ");
    strcat(sbuf, itoa(reg->fs, 16, 4, nbuf));
    strcat(sbuf, "    gs: ");
    strcat(sbuf, itoa(reg->gs, 16, 4, nbuf));
    dump(sbuf);
}
#else
// DumpCPURegister dumps the CPU registers to the screen or serial port
//   dest = 1 for screen
//   dest = 0 for serial port 0 (COM1)
//   ctrl = 1 to show configuration registers
void DumpCPURegisters(const struct cpu_reg *reg, int dest, int ctrl)
{
    char sbuf[80];
    char nbuf[20];
    void (*dump)(const char *);

    dump = println;
    if (dest) {
        dump = DisplayText;
    }

    strcpy(sbuf, "  eip: ");
    strcat(sbuf, itoa(reg->eip, 16, 8, nbuf));
    strcat(sbuf, "   esp: ");
    strcat(sbuf, itoa(reg->esp, 16, 8, nbuf));
    strcat(sbuf, "   ebp: ");
    strcat(sbuf, itoa(reg->ebp, 16, 8, nbuf));
    strcat(sbuf, " flags: ");
    strcat(sbuf, itoa(reg->eflags, 16, 8, nbuf));
    dump(sbuf);

    strcpy(sbuf, "  eax: ");
    strcat(sbuf, itoa(reg->eax, 16, 8, nbuf));
    strcat(sbuf, "   ebx: ");
    strcat(sbuf, itoa(reg->ebx, 16, 8, nbuf));
    strcat(sbuf, "   ecx: ");
    strcat(sbuf, itoa(reg->ecx, 16, 8, nbuf));
    strcat(sbuf, "   edx: ");
    strcat(sbuf, itoa(reg->edx, 16, 8, nbuf));
    dump(sbuf);

    strcpy(sbuf, "  esi: ");
    strcat(sbuf, itoa(reg->esi, 16, 8, nbuf));
    strcat(sbuf, "   edi: ");
    strcat(sbuf, itoa(reg->edi, 16, 8, nbuf));
    dump(sbuf);

    if (ctrl) {
        strcpy(sbuf, "  cr0: ");
        strcat(sbuf, itoa(reg->cr0, 16, 8, nbuf));
        strcat(sbuf, "   cr2: ");
        strcat(sbuf, itoa(reg->cr2, 16, 8, nbuf));
        strcat(sbuf, "   cr3: ");
        strcat(sbuf, itoa(reg->cr3, 16, 8, nbuf));
        strcat(sbuf, "   cr4: ");
        strcat(sbuf, itoa(reg->cr4, 16, 8, nbuf));
        dump(sbuf);
    }

    strcpy(sbuf, "   cs: ");
    strcat(sbuf, itoa(reg->cs, 16, 4, nbuf));
    strcat(sbuf, "  ss: ");
    strcat(sbuf, itoa(reg->ss, 16, 4, nbuf));
    strcat(sbuf, "   ds: ");
    strcat(sbuf, itoa(reg->ds, 16, 4, nbuf));
    strcat(sbuf, "  es: ");
    strcat(sbuf, itoa(reg->es, 16, 4, nbuf));
    strcat(sbuf, "   fs: ");
    strcat(sbuf, itoa(reg->fs, 16, 4, nbuf));
    strcat(sbuf, "  gs: ");
    strcat(sbuf, itoa(reg->gs, 16, 4, nbuf));
    dump(sbuf);
}
#endif
