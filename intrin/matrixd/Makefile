CC = gcc
CFLAGS = -O2 -mavx
LFLAGS =

matrixd: main.o matrixd.o
	$(CC) -o $@ $^ $(LFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) $<

matrixd.o: matrixd.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o matrixd
