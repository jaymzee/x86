#include "conio.h"

void WriteText(int x, int y, char *str, char attr)
{
    char *screen = (char *)0xB8000;

    int offset = 2 * (80 * y + x);
    while (*str) {
        screen[offset++] = *str++;
        screen[offset++] = attr;
    }
}

