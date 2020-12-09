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
void PIC_remap(int offset1, int offset2);

void PIC_MaskIRQ(unsigned char IRQline);
void PIC_UnmaskIRQ(unsigned char IRQline);
void PIC_sendEOI(unsigned char irq);
