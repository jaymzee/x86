SRC = hello.asm
BASE = $(basename $(SRC))
OBJ = $(addsuffix .bin,$(BASE))
IMG = $(addsuffix .img,$(BASE))

$(IMG): $(OBJ)
	python3 wrimg $<
$(OBJ): $(SRC)
	nasm -o $@ $<

clean:
	rm -f $(OBJ) $(IMG)
