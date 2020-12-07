ASFLAGS = -g -F dwarf -f elf
LDFLAGS = -m elf_i386 --oformat=binary

demo.img: stage0.bin stage1.bin
	python3 wrimg $@ $^
%.elf: %.asm
	nasm $(ASFLAGS) -o $@ $<
stage0.bin: stage0.elf
	ld $(LDFLAGS) -Ttext=0x7c00 -o $@ $^
stage1.bin: stage1.elf
	ld $(LDFLAGS) -Ttext=0x7e00 -o $@ $^
clean:
	rm -f demo.img *.elf *.bin
