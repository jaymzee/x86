hello.img: stage0.bin
	./mkboot $@ $<

%.bin: %.asm
	nasm -l $(addsuffix .lst,$(basename $@)) -o $@ $<

clean:
	rm -f *.bin *.lst hello.img
