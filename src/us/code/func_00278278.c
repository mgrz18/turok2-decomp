#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "globals/menu.h"



void *COnScreen__GetFontTexture(void);


/* Loads a 16-bit I texture (gDPLoadTextureBlock-style) for the image returned
 * by COnScreen__GetFontTexture. */
void COnScreen__SelectFont3(void) {
    void *image = COnScreen__GetFontTexture();

    if (image != NULL) {
        gLoadedFont = 3;
        GFX_CMD(0xFD900000, GBI_ADDR(image));            /* gDPSetTextureImage */
        GFX_CMD(0xF5900100, 0x07010040);            /* gDPSetTile */
        GFX_CMD(0xE6000000, 0);                     /* gDPLoadSync */
        GFX_CMD(0xF3000000, 0x0703F800);            /* gDPLoadBlock */
        GFX_CMD(0xE7000000, 0);                     /* gDPPipeSync */
        GFX_CMD(0xF5800300, 0x00010040);            /* gDPSetTile */
        GFX_CMD(0xF2000000, 0x0003C03C);            /* gDPSetTileSize */
    }
}
