CC = gcc
CFLAGS = -O2 -msse4
LFLAGS = -s

dot: main.c
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

clean:
	rm -f main.o dot
