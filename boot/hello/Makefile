hello.img: stage0.bin
	python3 wrimg $@ $<

%.bin: %.asm
	nasm -l $(addsuffix .lst,$(basename $@)) -o $@ $<

clean:
	rm -f *.bin *.lst hello.img
