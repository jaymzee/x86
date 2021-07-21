CC = gcc
CFLAGS =
LFLAGS =

mult: main.o mult.o
	$(CC) -o $@ $^ $(LFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) $<

mult.o: mult.asm
	nasm -felf64 $<

clean:
	rm -f *.o mult
