CFLAGS = -Os -m16 -mmanual-endbr -ffreestanding
ASFLAGS = -g -F dwarf -f elf
LDFLAGS = -m elf_i386 --nmagic

demo.img: stage0.bin stage1.bin
	python3 wrimg $@ $^

%.o: %.asm
	nasm $(ASFLAGS) -l $(addsuffix .lst,$(basename $@)) $<
%.o: %.c
	gcc $(CFLAGS) -c $<

stage0.bin: stage0.o
	ld $(LDFLAGS) -Ttext=0x7c00 --oformat=binary -o $@ $^
stage1.elf: stage1a.o stage1b.o
	ld $(LDFLAGS) -Ttext=0x7e00 -o $@ $^
stage1.bin: stage1.elf
	objcopy -O binary $< $@

clean:
	rm -f demo.img *.elf *.o *.lst *.bin
