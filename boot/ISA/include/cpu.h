#include <stdint.h>

#if __x86_64__
struct cpu_reg {
    uint64_t rax;   // 0
    uint64_t rcx;   // 8
    uint64_t rdx;   // 16
    uint64_t rbx;   // 24
    uint64_t rsp;   // 32
    uint64_t rbp;   // 40
    uint64_t rsi;   // 48
    uint64_t rdi;   // 56
    uint64_t r8;    // 64
    uint64_t r9;    // 72
    uint64_t r10;   // 80
    uint64_t r11;   // 88
    uint64_t r12;   // 96
    uint64_t r13;   // 104
    uint64_t r14;   // 112
    uint64_t r15;   // 120
    uint64_t rflags;// 128
    uint64_t rip;   // 136
    uint64_t cr0;   // 144
    uint64_t cr2;   // 152
    uint64_t cr3;   // 160
    uint64_t cr4;   // 168
    uint64_t cr8;   // 176
    uint16_t es;    // 184
    uint16_t cs;    // 186
    uint16_t ss;    // 188
    uint16_t ds;    // 190
    uint16_t fs;    // 192
    uint16_t gs;    // 194
};
#else
struct cpu_reg {
    uint32_t eax;   // 0
    uint32_t ecx;   // 4
    uint32_t edx;   // 8
    uint32_t ebx;   // 12
    uint32_t esp;   // 16
    uint32_t ebp;   // 20
    uint32_t esi;   // 24
    uint32_t edi;   // 28
    uint32_t eflags;// 32
    uint32_t eip;   // 36
    uint32_t cr0;   // 40
    uint32_t cr2;   // 44
    uint32_t cr3;   // 48
    uint32_t cr4;   // 52
    uint16_t es;    // 56
    uint16_t cs;    // 58
    uint16_t ss;    // 60
    uint16_t ds;    // 62
    uint16_t fs;    // 64
    uint16_t gs;    // 66
};
#endif

// DumpCPURegister dumps the CPU registers to the screen or serial port
//   dest = 1 for screen
//   dest = 0 for serial port 0 (COM1)
//   ctrl = 1 show cpu control registers
void DumpCPURegisters(char *sbuf, const struct cpu_reg *reg, int ctrl);
