qemu-system options
-fda floppy.img
-boot d -cdrom linux.iso
-boot order=adc
-drive format=raw,if=ide,file=linux.img
-accel hax
-nographic
-monitor stdio
-serial mon: stdio
-monitor telnet::45454,server,nowait
-device VGA,vgamem_mb=64
-curses
-m 256M

qemu curses mode
Alt+1 main window
Alt+2 monitor
Alt+3 serial 0

qemu GTK mode
Ctrl+Alt+1 main window
Ctrl+Alt+2 monitor
Ctrl+Alt+3 serial 0
