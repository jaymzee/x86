TARGET= boot.img
OBJS = main.o conio.o serial.o string.o
SOURCE = ../../isa/src
INCLUDE = ../../isa/include
CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall -Werror -m64 -Os -ffreestanding
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
$(TARGET): stage0.bin stage1.bin main.bin
	./mkboot $@ $^
%.bin: %.elf
	objcopy -O binary $< $@
stage0.elf: stage0.o
	$(LD) $(LDFLAGS) -T $(addsuffix .ld,$(basename $@)) -o $@ $^
stage1.elf: stage1.o
	$(LD) $(LDFLAGS) -T $(addsuffix .ld,$(basename $@)) -o $@ $^
main.elf: $(OBJS)
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
