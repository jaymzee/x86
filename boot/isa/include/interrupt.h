/*
 * intel 8259 interrupt controller
 */

#include <stdint.h>

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_RemapIVT(int offset1, int offset2);

// mask interrupt (disable interrupt)
void PIC_MaskIRQ(unsigned char irq);
// unmask interrupt (enable interrupt)
void PIC_UnmaskIRQ(unsigned char irq);
// issue nonspecific EOI
void PIC_SendEOI(unsigned char irq);
// Returns the combined value of the cascaded PICs irq request register
unsigned short PIC_GetIRR(void);
// Returns the combined value of the cascaded PICs in service register
unsigned short PIC_GetISR(void);

void NMI_Enable(void);
void NMI_Disable(void);

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
};
#else
struct IDT_entry {
    uint16_t    offset_lo;  // offset bits 0..15
    uint16_t    selector;   // a code segment selector in GDT or LDT
    uint8_t     zero;       // unused, set to zero
    uint8_t     type_attr;  // type and attributes
    uint16_t    offset_hi;  // offset bits 16..31
};
#endif

// descriptor for the entire IDT
struct IDTR {
    uint16_t    limit;      // size in bytes - 1
    uint64_t    offset;     // pointer to IDT
} __attribute__((packed));
