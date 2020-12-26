#include <conio.h>
#include <cpu.h>
#include <stdio.h>
#include <string.h>

const char *cpu_exc_str[32] = {
    "Divide-by-zero Error",         // 0x0
    "Debug",                        // 0x1
    "Non-maskable Interrupt",       // 0x2
    "Breakpoint",                   // 0x3
    "Overflow",                     // 0x4
    "Bound Range Exceeded",         // 0x5
    "Invalid Opcode",               // 0x6
    "Device Not Available",         // 0x7
    "Double Fault",                 // 0x8
    "Coprocessor Overrun",          // 0x9
    "Invalid TSS",                  // 0xa
    "Segment Not Present",          // 0xb
    "Stack Segment Fault",          // 0xc
    "General Protection Fault",     // 0xd
    "Page Fault",                   // 0xe
    "Reserved 0x0f",                // 0xf
    "x87 Floating-Point Exception", // 0x10
    "Alignment Check",              // 0x11
    "Machine Check",                // 0x12
    "SIMD Floating-Point Exception",// 0x13
    "Virtualization Exception",     // 0x14
    "Reserved 0x15",                // 0x15
    "Reserved 0x16",                // 0x16
    "Reserved 0x17",                // 0x17
    "Reserved 0x18",                // 0x18
    "Reserved 0x19",                // 0x19
    "Reserved 0x1a",                // 0x1a
    "Reserved 0x1b",                // 0x1b
    "Reserved 0x1c",                // 0x1c
    "Reserved 0x1d",                // 0x1d
    "Security Exception"            // 0x1e
};

void
IDT_IntGate(struct IDT_entry *descr, void (*hndlr)(void), int sel, int dpl)
{
#if __x86_64__
    descr->offset_hihi = (uint64_t)hndlr >> 32;
    descr->offset_hi = ((uint64_t)hndlr >> 16) & 0xffff;
    descr->offset_lo = (uint64_t)hndlr & 0xffff;
    descr->ist = 0;
#else
    descr->offset_hi = (uint32_t)hndlr >> 16;
    descr->offset_lo = (uint32_t)hndlr & 0xffff;
#endif
    descr->selector = sel;
    descr->type_attr = (dpl << 5) | 0x8E; // 32-bit interupt gate
    descr->zero = 0;
}

void
IDT_TrapGate(struct IDT_entry *descr, void (*hndlr)(void), int sel, int dpl)
{
#if __x86_64__
    descr->offset_hihi = (uint64_t)hndlr >> 32;
    descr->offset_hi = ((uint64_t)hndlr >> 16) & 0xffff;
    descr->offset_lo = (uint64_t)hndlr & 0xffff;
    descr->ist = 0;
#else
    descr->offset_hi = (uint32_t)hndlr >> 16;
    descr->offset_lo = (uint32_t)hndlr & 0xffff;
#endif
    descr->selector = sel;
    descr->type_attr = (dpl << 5) | 0x8F; // 32-bit trap gate
    descr->zero = 0;
}

#if __x86_64__
// DumpCPURegister dumps the CPU registers to the screen or serial port
//   dest = 1 for screen
//   dest = 0 for serial port 0 (COM1)
//   ctrl = 1 to show control registers
void DumpCPURegisters(char *sbuf, const struct cpu_reg *reg, int ctrl)
{
    char nbuf[20];

    strcpy(sbuf, "  rip: ");
    strcat(sbuf, itoa(reg->rip, 16, 16, nbuf));
    strcat(sbuf, "     rsp: ");
    strcat(sbuf, itoa(reg->rsp, 16, 16, nbuf));
    strcat(sbuf, "   flags: ");
    strcat(sbuf, itoa(reg->rflags, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, "  rax: ");
    strcat(sbuf, itoa(reg->rax, 16, 16, nbuf));
    strcat(sbuf, "     rbx: ");
    strcat(sbuf, itoa(reg->rbx, 16, 16, nbuf));
    strcat(sbuf, "     rcx: ");
    strcat(sbuf, itoa(reg->rcx, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, "  rdx: ");
    strcat(sbuf, itoa(reg->rdx, 16, 16, nbuf));
    strcat(sbuf, "     rsi: ");
    strcat(sbuf, itoa(reg->rsi, 16, 16, nbuf));
    strcat(sbuf, "     rdi: ");
    strcat(sbuf, itoa(reg->rdi, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, "  rbp: ");
    strcat(sbuf, itoa(reg->rbp, 16, 16, nbuf));
    strcat(sbuf, "      r8: ");
    strcat(sbuf, itoa(reg->r8, 16, 16, nbuf));
    strcat(sbuf, "      r9: ");
    strcat(sbuf, itoa(reg->r9, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, "  r10: ");
    strcat(sbuf, itoa(reg->r10, 16, 16, nbuf));
    strcat(sbuf, "     r11: ");
    strcat(sbuf, itoa(reg->r11, 16, 16, nbuf));
    strcat(sbuf, "     r12: ");
    strcat(sbuf, itoa(reg->r12, 16, 16, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, "  r13: ");
    strcat(sbuf, itoa(reg->r13, 16, 16, nbuf));
    strcat(sbuf, "     r14: ");
    strcat(sbuf, itoa(reg->r14, 16, 16, nbuf));
    strcat(sbuf, "     r15: ");
    strcat(sbuf, itoa(reg->r15, 16, 16, nbuf));
    strcat(sbuf, "\n");
    if (ctrl) {
        strcat(sbuf, "  cr0: ");
        strcat(sbuf, itoa(reg->cr0, 16, 16, nbuf));
        strcat(sbuf, "     cr2: ");
        strcat(sbuf, itoa(reg->cr2, 16, 16, nbuf));
        strcat(sbuf, "     cr3: ");
        strcat(sbuf, itoa(reg->cr3, 16, 16, nbuf));
        strcat(sbuf, "\n");
        strcat(sbuf, "  cr4: ");
        strcat(sbuf, itoa(reg->cr4, 16, 16, nbuf));
        strcat(sbuf, "     cr8: ");
        strcat(sbuf, itoa(reg->cr8, 16, 16, nbuf));
        strcat(sbuf, "\n");
    }
    strcat(sbuf, "   cs: ");
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
    strcat(sbuf, "\n");
}
#else
// DumpCPURegister dumps the CPU registers to the screen or serial port
//   dest = 1 for screen
//   dest = 0 for serial port 0 (COM1)
//   ctrl = 1 to show configuration registers
void DumpCPURegisters(char *sbuf, const struct cpu_reg *reg, int ctrl)
{
    char nbuf[20];

    strcpy(sbuf, "  eip: ");
    strcat(sbuf, itoa(reg->eip, 16, 8, nbuf));
    strcat(sbuf, "   esp: ");
    strcat(sbuf, itoa(reg->esp, 16, 8, nbuf));
    strcat(sbuf, "   ebp: ");
    strcat(sbuf, itoa(reg->ebp, 16, 8, nbuf));
    strcat(sbuf, " flags: ");
    strcat(sbuf, itoa(reg->eflags, 16, 8, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, "  eax: ");
    strcat(sbuf, itoa(reg->eax, 16, 8, nbuf));
    strcat(sbuf, "   ebx: ");
    strcat(sbuf, itoa(reg->ebx, 16, 8, nbuf));
    strcat(sbuf, "   ecx: ");
    strcat(sbuf, itoa(reg->ecx, 16, 8, nbuf));
    strcat(sbuf, "   edx: ");
    strcat(sbuf, itoa(reg->edx, 16, 8, nbuf));
    strcat(sbuf, "\n");
    strcat(sbuf, "  esi: ");
    strcat(sbuf, itoa(reg->esi, 16, 8, nbuf));
    strcat(sbuf, "   edi: ");
    strcat(sbuf, itoa(reg->edi, 16, 8, nbuf));
    strcat(sbuf, "\n");
    if (ctrl) {
        strcat(sbuf, "  cr0: ");
        strcat(sbuf, itoa(reg->cr0, 16, 8, nbuf));
        strcat(sbuf, "   cr2: ");
        strcat(sbuf, itoa(reg->cr2, 16, 8, nbuf));
        strcat(sbuf, "   cr3: ");
        strcat(sbuf, itoa(reg->cr3, 16, 8, nbuf));
        strcat(sbuf, "   cr4: ");
        strcat(sbuf, itoa(reg->cr4, 16, 8, nbuf));
        strcat(sbuf, "\n");
    }
    strcat(sbuf, "   cs: ");
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
    strcat(sbuf, "\n");
}
#endif

