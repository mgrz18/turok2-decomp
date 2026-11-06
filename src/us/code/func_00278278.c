#include "common.h"
#include "gfx.h"


extern Gfx *gDList;         /* display list head */
extern s32 gLoadedFont;

void *COnScreen__GetFontTexture(void);

#define GFX(a, b)                   \
    {                               \
        Gfx *_g = gDList++;     \
        _g->w0 = (a);               \
        _g->w1 = (b);               \
    }

/* Loads a 16-bit I texture (gDPLoadTextureBlock-style) for the image returned
 * by COnScreen__GetFontTexture. */
void COnScreen__SelectFont3(void) {
    void *image = COnScreen__GetFontTexture();

    if (image != NULL) {
        gLoadedFont = 3;
        GFX(0xFD900000, (u32)image);            /* gDPSetTextureImage */
        GFX(0xF5900100, 0x07010040);            /* gDPSetTile */
        GFX(0xE6000000, 0);                     /* gDPLoadSync */
        GFX(0xF3000000, 0x0703F800);            /* gDPLoadBlock */
        GFX(0xE7000000, 0);                     /* gDPPipeSync */
        GFX(0xF5800300, 0x00010040);            /* gDPSetTile */
        GFX(0xF2000000, 0x0003C03C);            /* gDPSetTileSize */
    }
}
