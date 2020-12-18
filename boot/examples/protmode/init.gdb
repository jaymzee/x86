# use this script to start qemu, connect to it, and debug code as 32/64 bit
add-symbol-file main.elf 0x8000
add-symbol-file stage1.elf 0x7e00
target remote | ./qemu -S -gdb stdio
