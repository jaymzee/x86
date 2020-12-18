#include <conio.h>
#include <system.h>
#include <vga.h>

#define TEXT_COLS   80
#define TEXT_ROWS   25

void ClearScreen(char attr)
{
    char *screen = (char *)0xB8000;
    for (int i = 0; i < 2000; i++) {
        screen[2*i] = ' ';
        screen[2*i+1] = attr;
    }
}

void SetCursorPos(int x, int y)
{
    int offset = TEXT_COLS * y + x;
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_LO);
    outb(VGA_CRTC_DATA, offset & 0xFF);
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_HI);
    outb(VGA_CRTC_DATA, (offset >> 8) & 0xFF);
}

void GetCursorPos(int *x, int *y)
{
    int offset = 0;
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_LO);
    offset = inb(VGA_CRTC_DATA);
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_HI);
    offset |= inb(VGA_CRTC_DATA) << 8;
    *y = offset / TEXT_COLS;
    *x = offset % TEXT_COLS;
}

void LocateText(int x, int y, const char *str, char attr)
{
    char *screen = (char *)0xB8000;
    int offset = 2 * (TEXT_COLS * y + x);
    while (*str) {
        screen[offset++] = *str++;
        screen[offset++] = attr;
    }
}

void DisplayText(const char *str, char attr)
{
    int x, y;
    GetCursorPos(&x, &y);
    LocateText(x, y++, str, attr);
    SetCursorPos(x, y);
}
