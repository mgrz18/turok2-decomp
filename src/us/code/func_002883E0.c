/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80291074_de_8.c, func_8029311C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "gfx.h"

extern s32 Render_SetRenderMode(s32);
extern void Render_SetCombineMode(u32);
void CEngineApp__DrawFilledRect(void *unused, u8 *color, s32 left, s32 top, s32 right, s32 bottom) {
    if (color[3] != 0) {
        { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
        { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (20) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x0)) << 20))); };
        Render_SetRenderMode(0x13);
        Render_SetCombineMode(0x19);
        { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((((unsigned int)(color[0]) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(color[1]) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(color[2]) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(color[3]) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
        {
            Gfx *cmd = gDList++;
            s32 x, y, x0, y0;
            u32 high, low, high0;
            x = right;
            if (x < 0) x = 0;
            y = bottom;
            high = (((unsigned int)(x) & (0xFFFFFFFFU >> (32 - (10)))) << (14));
            if (y < 0) y = 0;
            low = (((unsigned int)(y) & (0xFFFFFFFFU >> (32 - (10)))) << (2)) | (((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24));
            cmd->w0 = high | low;
            x0 = left;
            if (x0 < 0) x0 = 0;
            y0 = top;
            high0 = (((unsigned int)(x0) & (0xFFFFFFFFU >> (32 - (10)))) << (14));
            if (y0 < 0) y0 = 0;
            cmd->w1 = high0 | (((unsigned int)(y0) & (0xFFFFFFFFU >> (32 - (10)))) << (2));
        }
    }
}
