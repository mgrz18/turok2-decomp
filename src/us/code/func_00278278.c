#include "common.h"

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

extern Gfx *D_800D8E24;         /* display list head */
extern s32 D_800F56A8;

void *func_0027B4EC(void);

#define GFX(a, b)                   \
    {                               \
        Gfx *_g = D_800D8E24++;     \
        _g->w0 = (a);               \
        _g->w1 = (b);               \
    }

/* Loads a 16-bit I texture (gDPLoadTextureBlock-style) for the image returned
 * by func_0027B4EC. */
void func_00278278(void) {
    void *image = func_0027B4EC();

    if (image != NULL) {
        D_800F56A8 = 3;
        GFX(0xFD900000, (u32)image);            /* gDPSetTextureImage */
        GFX(0xF5900100, 0x07010040);            /* gDPSetTile */
        GFX(0xE6000000, 0);                     /* gDPLoadSync */
        GFX(0xF3000000, 0x0703F800);            /* gDPLoadBlock */
        GFX(0xE7000000, 0);                     /* gDPPipeSync */
        GFX(0xF5800300, 0x00010040);            /* gDPSetTile */
        GFX(0xF2000000, 0x0003C03C);            /* gDPSetTileSize */
    }
}
