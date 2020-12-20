#include <stdint.h>

// IDT entry descriptor
#if __WORDSIZE == 64
struct IDT_entry {
    uint16_t    offset_lo;  // offset bits 0..15
    uint16_t    selector;   // a code segment selector in GDT or LDT
    uint8_t     ist;        // Interrupt Stack Table offset
    uint8_t     type_attr;  // type and attributes
    uint16_t    offset_hi;  // offset bits 16..31
    uint32_t    offset_hihi;// offset bits 32..63
    uint32_t    zero;       // unused, set to zero
} __attribute__((packed));
#else
struct IDT_entry {
    uint16_t    offset_lo;  // offset bits 0..15
    uint16_t    selector;   // a code segment selector in GDT or LDT
    uint8_t     zero;       // unused, set to zero
    uint8_t     type_attr;  // type and attributes
    uint16_t    offset_hi;  // offset bits 16..31
} __attribute__((packed));
#endif

// descriptor for the entire IDT
#if __WORDSIZE == 64
struct IDTR {
    uint16_t    limit;      // size in bytes - 1
    uint32_t    offset;     // pointer to IDT
} __attribute__((packed));
#else
struct IDTR {
    uint16_t    limit;      // size in bytes - 1
    uint64_t    offset;     // pointer to IDT
} __attribute__((packed));
#endif

static inline void LoadIDT(struct IDTR *idtr)
{
    __asm__("lidt %0" :: "m"(*idtr));
    __asm__("sti");
}

void IDT_IntGate(struct IDT_entry *d, void (*hndlr)(void), int sel, int dpl);
void IDT_TrapGate(struct IDT_entry *d, void (*hndlr)(void), int sel, int dpl);
