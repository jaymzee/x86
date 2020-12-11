add-symbol-file stage1.elf 0x7e00
target remote | qemu-system-i386 -S -gdb stdio -m 16 -boot a -drive format=raw,if=floppy,file=demo.img,readonly
