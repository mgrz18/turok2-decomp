/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A7AA4_de_3.c, func_802A8710_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0027836C();
extern s32 D_800F56A8;
extern Gfx *D_800D8E24;
extern s32 func_0027B4EC(void);
extern Gfx *D_800D8E24;
extern s32 func_0027B4EC(void);
void func_0027836C(void) {
    Gfx *cmd;
    s32 v0;
    v0 = func_0027B4EC();
    if (v0 != 0) {
        D_800F56A8 = 1;
        cmd = D_800D8E24++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->words.w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((unsigned int)((u32) v0)); };
        cmd = D_800D8E24++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->words.w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->words.w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
        cmd = D_800D8E24++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->words.w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); };
        cmd = D_800D8E24++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->words.w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(2048) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
        cmd = D_800D8E24++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->words.w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); };
        cmd = D_800D8E24++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->words.w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->words.w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
        cmd = D_800D8E24++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->words.w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(60) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(28) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
    }
}
extern Gfx *D_800D8E24;
extern s32 func_0027B4EC(void);
