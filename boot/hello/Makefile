SRC = hello.asm
BASE = $(basename $(SRC))
BIN = $(addsuffix .bin,$(BASE))
IMG = $(addsuffix .img,$(BASE))

$(IMG): $(BIN)
	python3 wrimg $<
$(BIN): $(SRC)
	nasm -o $@ $<

clean:
	rm -f $(BIN) $(IMG)
