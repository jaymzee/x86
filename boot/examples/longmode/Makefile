TARGET= boot.img
OBJS = stage1.o serial.o string.o main.o
SOURCE = ../../isa/src
INCLUDE = ../../isa/include
CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall -Os -m64 -ffreestanding
# these options keep the binary very small
CFLAGS += -fno-pic -fno-exceptions -fno-asynchronous-unwind-tables
CFLAGS += -nostdinc -I $(INCLUDE)
ASFLAGS = -g -F dwarf -f elf64 -I $(addsuffix /,$(INCLUDE))
LDFLAGS = --nmagic  # --nmagic eliminates gaps between sections
GCCVERSION := $(shell gcc -dumpversion)
ifeq ($(shell expr $(GCCVERSION) \>= 8), 1)
    # disable CET endbr32 instructions in function entry point
    CFLAGS += -fcf-protection=return
endif
vpath %.c src
vpath %.c $(SOURCE)
vpath %.asm src

.PHONY: clean run debug
$(TARGET): stage0.bin stage1.bin
	./mkboot $@ $^
%.bin: %.elf
	objcopy -O binary $< $@
stage0.elf: stage0.o
	$(LD) $(LDFLAGS) -T stage0.ld -o $@ $^
stage1.elf: $(OBJS)
	$(LD) $(LDFLAGS) -T stage1.ld -o $@ $^
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
