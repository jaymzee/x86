to disassemble the mbr:

    objdump -D -b binary -m i386 -M addr16,data16

or with netwide assembler

    ndisasm -b16 -o7c00h -a -s7c3eh mbr

-b = Specifies 16-, 32- or 64-bit mode. The default is 16-bit mode.
-o = Specifies the notional load address for the file. This option causes
ndisasm to get the addresses it lists down the left hand margin, and the
target addresses of PC-relative jumps and calls, right.
-a = Enables automatic (or intelligent) sync mode, in which ndisasm will
attempt to guess where synchronisation should be performed, by means of
examining the target addresses of the relative jumps and calls it disassembles.
-s = Manually specifies a synchronisation address, such that ndisasm will not
output any machine instruction which encompasses bytes on both sides of the
address. Hence the instruction which starts at that address will be correctly
disassembled.

to compile c code with gcc for 16 bit

    gcc -m16 -fnopie -nostdlib -ffreestanding
