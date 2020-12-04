demo.img: boot.bin boot2.bin
	python3 wrimg $@ $^
boot.bin: boot.asm
	nasm -o $@ $<
boot2.bin: boot2.asm
	nasm -o $@ $<
clean:
	rm -f demo.img boot.bin boot2.bin
