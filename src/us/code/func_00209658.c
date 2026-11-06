/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026AC38_de.c, func_8026AC38_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "gfx.h"

extern void Texture_InvalidateCache();
extern Gfx *gDList;
extern char D_800AB680;
extern u32 gFogGeometryMode;
extern s32 Render_SetRenderMode(s32);
void func_00209658(void) {
    Gfx *cmd;
    Texture_InvalidateCache();
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((unsigned int)(&D_800AB680)); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(1024) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(124) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(124) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xD7) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (11)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (8)) | (((unsigned int)(((unsigned int)(1))) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->w1 = ((((unsigned int)(2048) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(2048) & (0xFFFFFFFFU >> (32 - (16)))) << (0))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (14) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x0)) << 14))); };
    cmd = gDList++;
    { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xD9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(~(0)) & (0xFFFFFFFFU >> (32 - (24)))) << (0))); _gbi->w1 = ((unsigned int)(gFogGeometryMode | 0x00260404)); };
    Render_SetRenderMode(0x1D);
}
