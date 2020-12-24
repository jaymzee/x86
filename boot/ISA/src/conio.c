#include <conio.h>
#include <string.h>
#include <system.h>
#include <vga.h>

#define TEXT_ATTR   0x07
#define TEXT_COLS   80
#define TEXT_ROWS   25
#define VIDEOMEM    0xB8000

// 8042 PS/2 Controller
#define KB_DATA     0x60    // Read/Write
#define KB_CMD      0x64    // Write
#define KB_STATUS   0x64    // Read

static char *vga_text = (char *)VIDEOMEM;

void ClearText(unsigned char attr)
{
    unsigned short *vm = (unsigned short *)VIDEOMEM;
    for (int i = 0; i < 2000; i++) {
        vm[i] = attr << 8 | ' ';
    }
    SetTextCursor(0, 0);
}

void SetTextCursor(int x, int y)
{
    int offset = TEXT_COLS * y + x;
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_LO);
    outb(VGA_CRTC_DATA, offset & 0xFF);
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_HI);
    outb(VGA_CRTC_DATA, (offset >> 8) & 0xFF);
}

void GetTextCursor(int *x, int *y)
{
    int offset = 0;
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_LO);
    offset = inb(VGA_CRTC_DATA);
    outb(VGA_CRTC_ADDR, CRTC_CUR_LOC_HI);
    offset |= inb(VGA_CRTC_DATA) << 8;
    *y = offset / TEXT_COLS;
    *x = offset % TEXT_COLS;
}

void LocateText(int x, int y, const char *str)
{
    int offset = 2 * (TEXT_COLS * y + x);
    while (*str) {
        vga_text[offset] = *str++;
        offset += 2;
    }
}

static void ScrollText()
{
    int char_count = TEXT_COLS * (TEXT_ROWS - 1);
    int byte_count = 2 * char_count;
    memcpy(vga_text, vga_text + 2 * TEXT_COLS, byte_count);
    for (int i = 0; i < TEXT_COLS; i++) {
        vga_text[byte_count + 2 * i] = ' ';
    }
}

void WriteText(const char *str)
{
    int x, y, offset;
    char ch;

    GetTextCursor(&x, &y);
    offset = 2 * (TEXT_COLS * y + x);
    while ((ch = *str++) != '\0') {
        if (ch > 31 && ch < 127) {
            vga_text[offset] = ch;
            offset += 2;
            x++;
        } else if (ch == 10) {
            x = 0;
            y++;
            while (y > TEXT_ROWS - 1) {
                y--;
                ScrollText();
            }
            // recalculate offset
            offset = 2 * (TEXT_COLS * y + x);
        }
    }
    SetTextCursor(x, y);
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

int ScanKeyboard(void)
{
    int status = inb(KB_STATUS);
    if (status & 1) {
        return inb(KB_DATA);
    }
    return -1; // kb buffer empty
}
