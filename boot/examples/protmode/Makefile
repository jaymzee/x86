TARGET = boot.img
OBJECTS = main.o conio.o serial.o string.o interrupt.o isr.o
SOURCE = ../../isa/src
INCLUDE = ../../isa/include
CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall -Werror -m32  -ffreestanding
# these options keep the binary very small
CFLAGS += -fno-pic -fno-exceptions -fno-asynchronous-unwind-tables
CFLAGS += -nostdinc -I $(INCLUDE) -include _default.h
ASFLAGS = -g -F dwarf -f elf -I $(addsuffix /,$(INCLUDE))
LDFLAGS = -m elf_i386 --nmagic  # --nmagic eliminates gaps between sections
GCCVERSION := $(shell gcc -dumpversion)
ifeq ($(shell expr $(GCCVERSION) \>= 8), 1)
    # disable CET endbr32 instructions in function entry point
    CFLAGS += -fcf-protection=return
endif
vpath %.asm src
vpath %.c src
vpath %.c $(SOURCE)

.PHONY: clean run debug
$(TARGET): boot.bin main.bin
	./mkboot $@ $^
%.bin: %.elf
	objcopy -O binary $< $@
boot.elf: boot.o
	$(LD) $(LDFLAGS) -T $(addsuffix .ld,$(basename $@)) -o $@ $^
main.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -T $(addsuffix .ld,$(basename $@)) -o $@ $^
%.o: %.asm
	nasm $(ASFLAGS) -o $@ -l $(addsuffix .lst,$(basename $@)) $<
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	@$(RM) $(TARGET) *.elf *.o *.lst *.bin
run: $(TARGET)
	@./qemu -display curses
debug: $(TARGET)
	gdb -x init.gdb
