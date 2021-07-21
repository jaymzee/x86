ASFLAGS = -felf
LDFLAGS = -s -n -melf_i386

tiny: tiny.o
	ld $(LDFLAGS) -o $@ $<

tiny.o: tiny.asm
	nasm $(ASFLAGS) -o $@ $<

clean:
	rm -f tiny.o tiny
