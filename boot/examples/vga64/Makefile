TARGET = boot.img
OBJECTS = main.o stdio.o conio.o kbd_US.o serial.o string.o graphics.o
SOURCE = ../../ISA/src
INCLUDE = ../../ISA/include
CC = gcc
CFLAGS = -g -Os -std=c99 -pedantic -Wall -Werror
CFLAGS += -m64 -ffreestanding -mno-red-zone
CFLAGS += -fno-pic -fno-exceptions -fno-asynchronous-unwind-tables
CFLAGS += -nostdinc -I $(INCLUDE) -include _default.h
ASFLAGS = -g -F dwarf -f elf64 -I $(addsuffix /,$(INCLUDE))
LDFLAGS = --nmagic  # --nmagic eliminates gaps between sections
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
	$(LD) $(LDFLAGS) -T boot.ld -o $@ $^
main.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -T main.ld -o $@ $^
%.o: %.asm
	nasm $(ASFLAGS) -o $@ -l $(addsuffix .lst,$(basename $@)) $<
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	@$(RM) $(TARGET) *.elf *.o *.lst *.bin
run: $(TARGET)
	@./qemu
debug: $(TARGET)
	gdb -x init.gdb
