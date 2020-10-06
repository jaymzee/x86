ASFLAGS=
LDFLAGS=-s

hello: hello.o
	ld $(LDFLAGS) -o $@ $<

hello.o: hello.s
	as $(ASFLAGS) -o $@ $<

clean:
	rm hello hello.o
