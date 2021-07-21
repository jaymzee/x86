ASFLAGS = -felf
LDFLAGS = -n -melf_i386 -s

tiny: tiny.o
	ld $(LDFLAGS) -o $@ $<

tiny.o: tiny.asm
	nasm $(ASFLAGS) -o $@ $<

clean:
	rm -f tiny.o tiny
