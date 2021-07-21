ASFLAGS=
LDFLAGS=-s

hello: hello.o
	ld $(LDFLAGS) -o $@ $<

hello.o: hello.s
	as $(ASFLAGS) -o $@ $<

hello.s: hello.S common.S
	cpp hello.S > hello.s

clean:
	rm -f hello hello.o hello.s
