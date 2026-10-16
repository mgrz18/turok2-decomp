/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A7AA4_de_5.c, func_802A9234_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef union Gfx {
    struct { u32 w0; u32 w1; } words;
    u64 force_structure_alignment;
} Gfx;
extern s32 D_800C2038;
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
extern s32 D_800C2038;
extern void func_00207A70(s32 arg0);
extern void func_00207FEC(s32 arg0);
void func_00279028(s32 color) {
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (20) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)((((unsigned int)(0x1)) << 20))); };
    func_00207A70(0x1B);
    func_00207FEC(0x19);
    {
        Gfx *cmd = D_800D8E24++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->words.w0 = ((((unsigned int)(0xFB) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = ((((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(color) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
    }
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xD7) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (11)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (8)) | (((unsigned int)(((unsigned int)(1))) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->words.w1 = ((((unsigned int)(32768) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(32768) & (0xFFFFFFFFU >> (32 - (16)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (14) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)((((unsigned int)(0x0)) << 14))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (19) - (1)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)((((unsigned int)(0x0)) << 19))); };
    if (D_800C2038 == 0) { { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (12) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)((((unsigned int)(0x0)) << 12))); }; } else { { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (12) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)((((unsigned int)(0x2)) << 12))); }; }
}
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
