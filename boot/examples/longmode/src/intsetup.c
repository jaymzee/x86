#include <interrupt.h>
#include <string.h>
#include "intsetup.h"

#define IDT_ENTRIES 256
#define IDT_ADDRESS 0x1000
#define IDT_SIZE (IDT_ENTRIES * sizeof(struct IDT_entry))

extern void GPFaultHandler(void);
extern void PageFaultHandler(void);
extern void KeyboardHandler(void);
extern void TimerHandler(void);

// this sets up the interrupt descriptor tables then enables interrupts
void EnableInterrupts(void)
{
    struct IDT_entry *idt = (void *)IDT_ADDRESS;
    struct IDTR *idtr = (void *)(IDT_ADDRESS + IDT_SIZE);

    // move IRQs to interrupts 0x20-2F
    // so they do not conflict with CPU exceptions
    PIC_RemapIVT(0x20, 0x28);
    PIC_MaskAllIRQ(); // mask all IRQ

    memset(idt, 0, IDT_SIZE); // clear IDT

    IDT_TrapGate(idt + 0x0d, GPFaultHandler, 8, 0);
    IDT_TrapGate(idt + 0x0e, PageFaultHandler, 8, 0);
    IDT_IntGate(idt + 0x21, KeyboardHandler, 8, 0);
    IDT_IntGate(idt + 0x20, TimerHandler, 8, 0);

    idtr->offset = (uint64_t)idt;
    idtr->limit = IDT_SIZE - 1;

    LoadIDT(idtr); // also enables cpu interrupts

    PIC_UnmaskIRQ(1); // enable keyboard IRQ
    PIC_UnmaskIRQ(0); // enable timer IRQ
}
