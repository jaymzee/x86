#include <conio.h>
#include <string.h>
#include <system.h>
#include <vga.h>

#define TEXT_ATTR   0x07
#define TEXT_COLS   80
#define TEXT_ROWS   25
#define VIDEOMEM    0xB8000

void ClearScreen(unsigned char attr)
{
    unsigned short *screen = (unsigned short *)VIDEOMEM;
    for (int i = 0; i < 2000; i++) {
        screen[i] = attr << 8 | ' ';
    }
    SetCursorPos(0, 0);
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

void LocateText(int x, int y, const char *str, unsigned char attr)
{
    char *screen = (char *)VIDEOMEM;
    int offset = 2 * (TEXT_COLS * y + x);
    while (*str) {
        screen[offset] = *str++;
        offset += 2;
    }
}

// crude but good enough for simple display
void DisplayText(const char *str)
{
    char *screen = (char *)VIDEOMEM;
    int x, y;
    GetCursorPos(&x, &y);
    LocateText(x, y++, str, TEXT_ATTR);
    if (y > TEXT_ROWS - 1) {
        int char_count = TEXT_COLS * (TEXT_ROWS - 1);
        int byte_count = 2 * char_count;
        memcpy(screen, screen + 2 * TEXT_COLS, byte_count);
        for (int i = 0; i < TEXT_COLS; i++) {
            screen[byte_count + 2 * i] = ' ';
        }
        y = TEXT_ROWS - 1;
    }
    SetCursorPos(x, y);
}

void EnableBlink(void)
{
    inb(VGA_INPUT_STAT1); // this will reset index/data flip-flop
    outb(VGA_ATTR_ADDR, VGA_ATTR_MODE_CTRL); // select attr mode control reg
    outb(VGA_ATTR_DATA_WR, 0x08 | inb(VGA_ATTR_DATA_RD)); // turn on bit 3
}

void DisableBlink(void)
{
    inb(VGA_INPUT_STAT1); // this will reset index/data flip-flop
    outb(VGA_ATTR_ADDR, VGA_ATTR_MODE_CTRL); // select attr mode control reg
    outb(VGA_ATTR_DATA_WR, 0xf7 & inb(VGA_ATTR_DATA_RD)); // turn off bit 3
}
