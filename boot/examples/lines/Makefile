TARGET = boot.img
OBJECTS = stage1.o serial.o string.o graphics.o main.o
SOURCE = ../../isa/src
INCLUDE = ../../isa/include
CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall -Os -m32 -ffreestanding
# these options keep the binary very small
CFLAGS += -fno-pic -fno-exceptions -fno-asynchronous-unwind-tables
CFLAGS += -nostdinc -I $(INCLUDE)
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
$(TARGET): stage0.bin stage1.bin
	./mkboot $@ $^
%.bin: %.elf
	objcopy -O binary $< $@
stage0.elf: stage0.o
	$(LD) $(LDFLAGS) -T stage0.ld -o $@ $^
stage1.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -T stage1.ld -o $@ $^
%.o: %.asm
	nasm $(ASFLAGS) -o $@ -l $(addsuffix .lst,$(basename $@)) $<
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	@$(RM) $(TARGET) *.elf *.o *.lst *.bin
run: $(TARGET)
	@./qemu
debug: $(TARGET)
	gdb -x start.gdb
