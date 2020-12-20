/*
 * intel 8259 interrupt controller
 */

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_RemapIVT(int offset1, int offset2);

// mask interrupt (disable IRQ line)
void PIC_MaskIRQ(unsigned char irq);
// mask all interrupts (disable IRQ lines on both PICs)
void PIC_MaskAllIRQ(void);
// unmask interrupt (enable IRQ line)
void PIC_UnmaskIRQ(unsigned char irq);
// issue nonspecific EOI
void PIC_SendEOI(unsigned char irq);
// Returns the combined value of the cascaded PICs irq request register
unsigned short PIC_GetIRR(void);
// Returns the combined value of the cascaded PICs in service register
unsigned short PIC_GetISR(void);
