#include <string.h>
#include <sys/cpu.h>
#include <sys/interrupt.h>
#include "intsetup.h"
#include "isr.h"
#include "traps.h"

#define CS 0x08
#define DPL 0

// this sets up the interrupt descriptor tables then enables interrupts
void EnableInterrupts(void)
{
    struct IDT_entry *idt = (void *)IDT_ADDRESS;
    struct IDTR *idtr = (void *)(IDT_ADDRESS + IDT_SIZE);

    // move IRQs to interrupts 0x20-2F
    // so they do not conflict with CPU exceptions
    PIC_OffsetIRQ(0x20, 0x28);
    PIC_MaskAllIRQ(); // mask all IRQ

    memset(idt, 0, IDT_SIZE); // clear IDT

    IDT_TrapGate(idt + EXC_DIVBYZERO, DivbyzeroHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_DEBUG, DebugHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_NMI, NMIHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_BREAKPOINT, BreakpointHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_OVERFLOW, OverflowHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_BOUND, BoundRangeHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_OPCODE, InvalidOpcodeHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_DBL_FAULT, DoubleFaultHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_TSS, InvalidTSSHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_SEGMENT, SegNotPresentHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_STACK, StackSegFaultHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_GPF, GPFaultHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_PF, PageFaultHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_X87_FLOAT, x87FPExceptHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_ALIGN, AlignCheckHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_MACHINE, MachineCheckHandler, CS, DPL);
    IDT_TrapGate(idt + EXC_SIMD_FLOAT, SIMDFPExceptHandler, CS, DPL);

    IDT_IntGate(idt + 0x21, KeyboardHandler, CS, DPL);
    IDT_IntGate(idt + 0x20, TimerHandler, CS, DPL);

    idtr->offset = (uint64_t)idt;
    idtr->limit = IDT_SIZE - 1;

    LoadIDT(idtr); // also enables cpu interrupts

    PIC_UnmaskIRQ(1); // enable keyboard IRQ
    PIC_UnmaskIRQ(0); // enable timer IRQ
}
