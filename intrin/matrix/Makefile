CC = gcc
CFLAGS = -O2 -msse4
LFLAGS =

matrix: main.o matrix.o
	$(CC) -o $@ $^ $(LFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) $<

matrix.o: matrix.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o matrix
