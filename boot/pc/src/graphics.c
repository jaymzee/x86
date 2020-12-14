/* VGA Mode 13h drawing functions */

static unsigned char *video_ram = (unsigned char *)0xA0000;

void DrawPixel(int x, int y, unsigned char color)
{
    video_ram[320 * y + x] = color;
}

void DrawLine(int x1, int y1, int x2, int y2, unsigned char color)
{
    for (int i = 0; i < 150; i++) {
        video_ram[320 * i + i] = color;
    }
}
