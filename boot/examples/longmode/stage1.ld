ENTRY(_start);
SECTIONS
{
    . = 0x7E00;
    .sig : AT(0x7FFE)
    {
        SHORT(0xaa55);
    }
}
