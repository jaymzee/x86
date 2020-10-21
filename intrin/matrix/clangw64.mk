CC = clang
CFLAGS = -std=c99 -pedantic -Wall -O2
LFLAGS =

all: test4x4d.exe test4x4s.exe

test4x4d.exe: test4x4d.o matrix4x4d.o print_avx.o
	$(CC) -o $@ $^ $(LFLAGS)

test4x4s.exe: test4x4s.o matrix4x4s.o print_sse.o
	$(CC) -o $@ $^ $(LFLAGS)

test4x4d.o: test4x4d.c
	$(CC) -c $(CFLAGS) -mavx2 $<

test4x4s.o: test4x4s.c
	$(CC) -c $(CFLAGS) -mavx $<

matrix4x4d.o: matrix4x4d.c matrix4x4d.h
	$(CC) -c $(CFLAGS) -mavx2 $<

matrix4x4s.o: matrix4x4s.c matrix4x4s.h
	$(CC) -c $(CFLAGS) -msse4 $<

print_sse.o: print_sse.c print_sse.h
	$(CC) -c $(CFLAGS) -msse $<

print_avx.o: print_avx.c print_avx.h
	$(CC) -c $(CFLAGS) -mavx $<

clean:
	rm -f *.o test4x4d test4x4s
