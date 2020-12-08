#define outb(port, value) __asm("outb %b0, %w1" : : "a" (value), "d" (port))
