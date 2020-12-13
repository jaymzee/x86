TARGET= boot.img
OBJS = stage1.o serial.o string.o time.o main.o
CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall -Os -m32 -ffreestanding -I include
# these options keep the binary very small
CFLAGS += -fno-pic -fno-exceptions -fno-asynchronous-unwind-tables
ASFLAGS = -g -F dwarf -f elf
LDFLAGS = -m elf_i386 --nmagic  # --nmagic eliminates gaps between sections
GCCVERSION := $(shell gcc -dumpversion)
ifeq ($(shell expr $(GCCVERSION) \>= 8), 1)
    # disable CET endbr32 instructions in function entry point
    CFLAGS += -fcf-protection=return
endif
vpath %.c src
vpath %.asm src

.PHONY: clean run debug
$(TARGET): stage0.bin stage1.bin
	./mkboot $@ $^
%.bin: %.elf
	objcopy -O binary $< $@
stage0.elf: stage0.o
	$(LD) $(LDFLAGS) -Ttext=0x7c00 -o $@ $^
stage1.elf: $(OBJS)
	$(LD) $(LDFLAGS) -Ttext=0x7e00 -o $@ $^
%.o: %.asm
	nasm $(ASFLAGS) -o $@ -l $(addsuffix .lst,$(basename $@)) $<
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	@$(RM) $(TARGET) *.elf *.o *.lst *.bin
run: $(TARGET)
	@./qemu -display curses
debug: $(TARGET)
	gdb -x start.gdb
