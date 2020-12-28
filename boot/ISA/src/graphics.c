/* VGA Mode 13h drawing functions */

static unsigned char *video_ram = (unsigned char *)0xA0000;

void DrawPixel(int x, int y, unsigned char color)
{
    video_ram[320 * y + x] = color;
}

// Midpoint algorithm for 2D line
void DrawLine(int x0, int y0, int x1, int y1, unsigned char color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int x = x0;
    int y = y0;
    int incx = 1;
    int incy = 1;

    if (dx < 0) {
        dx = -dx;
        incx = -1;
    }

    if (dy < 0) {
        dy = -dy;
        incy = -1;
    }

    video_ram[320 * y + x] = color;
    if (dx > dy) {
        const int incrE = dy * 2;
        const int incrSE = (dy - dx) * 2;
        int d = dy * 2 - dx;
        while (x < x1) {
            if (d <= 0) {
                d += incrE;
                x += incx;
            } else {
                d += incrSE;
                x += incx;
                y += incy;
            }
            video_ram[320 * y + x] = color;
        }
    } else {
        const int incrS = dx * 2;
        const int incrSE = (dx - dy) * 2;
        int d = dx * 2 - dy;
        while (y < y1) {
            if (d <= 0) {
                d += incrS;
                y += incy;
            } else {
                d += incrSE;
                x += incx;
                y += incy;
            }
            video_ram[320 * y + x] = color;
        }
    }
}
