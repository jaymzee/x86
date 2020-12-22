/* VGA Mode 13h drawing functions */

static unsigned char *video_ram = (unsigned char *)0xA0000;

void DrawPixel(int x, int y, unsigned char color)
{
    video_ram[320 * y + x] = color;
}

// Midpoint algorithm for 2D line
void DrawLine(int x0, int y0, int x1, int y1, unsigned char color)
{
    const int dx = x1 - x0;
    const int dy = y1 - y0;
    const int incrE = dy * 2;
    const int incrNE = (dy - dx) * 2;
    int d = dy * 2 - dx;
    int x = x0;
    int y = y0;

    video_ram[320 * y + x] = color;
    while (x < x1) {
        if (d <= 0) {
            d += incrE;
            x++;
        } else {
            d += incrNE;
            x++;
            y++;
        }
        video_ram[320 * y + x] = color;
    }
}
