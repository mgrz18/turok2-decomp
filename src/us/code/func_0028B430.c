typedef int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

extern u32 D_800B7490;          /* debug console cursor column */
extern u32 D_800B7494;          /* debug console cursor row */
extern u16 D_800B7498;          /* background colour */
extern u8 D_800B6EA0[0x60][16]; /* 8x16 font, ASCII 0x20-0x7F */
extern s32 D_800C2030;          /* framebuffer width */
extern s32 D_800C2034;          /* framebuffer height */
extern u16 *D_8011F180;         /* framebuffer */
extern u16 D_8011F184;          /* foreground colour */

static __inline__ void newline(void) {
    D_800B7490 = 2;
    D_800B7494++;
    if (D_800B7494 >= D_800C2034 / 16 - 1) {
        D_800B7494 = 0;
    }
}

/* Debug console: draws one character at the cursor and advances it. */
void func_0028B430(s32 c) {
    s32 row;
    s32 col;
    s32 width;
    u16 *fb;
    s32 y;
    s32 x;
    u16 *pixel;

    switch (c) {
    case '\r':
        D_800B7490 = 2;
        break;
    case '\t':
        D_800B7490 += 8;
        if (D_800B7490 >= D_800C2030 / 8 - 1) {
            newline();
        }
        break;
    case '\f':
        D_800B7494 = 0;
        D_800B7490 = 2;
        break;
    case '\n':
        newline();
        break;
    default:
        c -= 0x20;
        if (c < 0) {
            c = 0x1F;
        } else if (c >= 0x60) {
            c = 0x1F;
        }
        width = D_800C2030;
        fb = D_8011F180;
        y = D_800B7494 * 16;
        x = D_800B7490 * 8;
        /* The row loop is a goto loop: as a for loop GCC hoists the glyph
         * offset out of it, which the ROM does not. */
        row = 0;
    nextRow:
        for (col = 0; col < 8; col++) {
            pixel = (u16 *)(((y + row) * width + (x + col)) * 2 + (u32)fb);
            if (!((D_800B6EA0[c][row] >> (7 - col)) & 1)) {
                *pixel = D_800B7498;
            } else {
                *pixel = D_8011F184;
            }
        }
        if (++row < 16) {
            goto nextRow;
        }
        if (++D_800B7490 >= D_800C2030 / 8 - 1) {
            newline();
        }
        break;
    }
}
