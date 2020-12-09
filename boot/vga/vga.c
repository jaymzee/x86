// only color vga is supported

#define VGA_MISC_OUT_WR     0x3C2   // write only
#define VGA_INPUT_STAT0     0x3C2   // read only
#define VGA_MISC_OUT_RD     0x3CC
#define VGA_GFX_ADDR        0x3CE
#define VGA_GFX_DATA        0x3CF
#define VGA_FEAT_CTRL_RD    0x3CA
#define VGA_FEAT_CTRL_WR    0x3DA   // write only
#define VGA_INPUT_STAT1     0x3DA   // read only

#define GFX_SET_RST         0
#define GFX_EN_SET_RST      1
#define GFX_COLOR_CMP       2
#define GFX_DATA_ROT        3
#define GFX_RD_MAP_SEL      4
#define GFX_MODE            5
#define GFX_MISC            6
#define GFX_COLOR_DC        7
#define GFX_BIT_MASK        8
