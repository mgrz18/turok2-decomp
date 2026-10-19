/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80268CE0_de.c, func_8026992C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u32 D_800AB880;
extern u32 D_800AB580[];
extern int D_800AB884;
extern Gfx *D_800D8E24;
extern s32 D_800AB884;
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
extern Gfx D_800AB458[], D_800AB4E0[];
extern s32 D_800AB884;
extern void func_00207A54(void);
void func_002086BC(void *matrix, s32 pass, u32 alpha) {
    Gfx *list;
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((unsigned int)(D_800AB580)); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->words.w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x1)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x1)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(127) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(1024) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->words.w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x1)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x1)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(15 << 2) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(15 << 2) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xFA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(alpha) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xDA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (5)))) << (19)) | (((unsigned int)(((((unsigned int)(0x0)) << 2) | (((unsigned int)(0x1)) << 1) | (((unsigned int)(0x0)) << 0)) ^ (((unsigned int)(0x1)) << 0)) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)(matrix)); };
    list = D_800AB458;
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xDE) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (8)))) << (16))); _gbi->words.w1 = ((unsigned int)(({ if (pass != 0) list = D_800AB4E0; list; }))); };
    D_800AB884 = -1;
    D_800AB880 = -1;
    func_00207A54();
}
extern Gfx *D_800D8E24;
