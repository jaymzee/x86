#define outb(port, value) __asm__ __volatile__("outb %b0,%w1": :"a" (value), "Nd" (port));

inline unsigned char
inb(unsigned short port)
{
    unsigned char v;
    __asm__ __volatile__ ("inb %w1,%0":"=a" (v):"Nd" (port));
    return v;
}
