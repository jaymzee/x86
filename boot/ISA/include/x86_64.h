#include <stdint.h>

struct x86_64_regs {
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
    uint64_t rflags; // 128
    uint64_t rip;   // 136
    uint16_t es;    // 144
    uint16_t cs;    // 146
    uint16_t ss;    // 148
    uint16_t ds;    // 150
    uint16_t fs;    // 152
    uint16_t gs;    // 154
} __attribute__((packed));
