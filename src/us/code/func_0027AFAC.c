/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A7AA4_de_7.c, func_802AAA54_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void func_0027AFAC(s32 x0,s32 y0,s32 x1,s32 y1,u8 r,u8 g,u8 b,u8 a) {
    extern Gfx *D_800D8E24;
 { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); };
 { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xFA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((((unsigned int)(r) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(g) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(b) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(a) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
 { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((x1))>((0))?((x1)):((0))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((y1))>((0))?((y1)):((0))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(((x0))>((0))?((x0)):((0))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((y0))>((0))?((y0)):((0))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
}
