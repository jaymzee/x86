CFLAGS = -g -std=c99 -pedantic -Wall -Os -m16 -ffreestanding -fno-pic -fno-exceptions -fno-asynchronous-unwind-tables
ASFLAGS = -g -F dwarf -f elf
LDFLAGS = -m elf_i386 --nmagic  # --nmagic eliminates gaps between sections
GCCVERSION := $(shell gcc -dumpversion)
ifeq ($(shell expr $(GCCVERSION) \>= 8), 1)
    # disable CET endbr32 instructions in function entry point
    CFLAGS += -fcf-protection=return
endif

demo.img: stage0.bin stage1.bin
	python3 wrimg $@ $^
%.bin: %.elf
	objcopy -O binary $< $@
stage0.elf: stage0.o
	ld $(LDFLAGS) -Ttext=0x7c00 -o $@ $^
stage1.elf: stage1.o serial.o util.o pic.o main.o
	ld $(LDFLAGS) -Ttext=0x7e00 -o $@ $^
%.o: %.asm
	nasm $(ASFLAGS) -l $(addsuffix .lst,$(basename $@)) $<
%.o: %.c
	gcc $(CFLAGS) -c $<
clean:
	rm -f demo.img *.elf *.o *.lst *.bin
