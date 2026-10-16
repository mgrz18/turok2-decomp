/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A7AA4_de_5.c, func_802A9EE8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef union Gfx {
    struct { u32 w0; u32 w1; } words;
    u64 force_structure_alignment;
} Gfx;
extern float D_800A83F0;
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
extern Gfx *D_800D8E24;
extern f32 D_800A83F0;
void func_0027A4DC(void *pTexture, f32 XPos, f32 YPos, int TextureWidth, int TextureHeight, f32 ScaleX, f32 ScaleY) {
    int dsdx;
    int dtdy;
    int xl, yl, xh, yh;
    dsdx = 1 << 10;
    if (ScaleX != 0.0f) {
        dsdx = dsdx / ScaleX;
    }
    dtdy = 1 << 10;
    if (ScaleY != 0.0f) {
        dtdy = dtdy / ScaleY;
    }
    xl = XPos * D_800A83F0;
    xh = (XPos + TextureWidth * ScaleX) * D_800A83F0;
    yl = YPos * D_800A83F0;
    yh = (YPos + TextureHeight * ScaleY) * D_800A83F0;
    { { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((unsigned int)((u32)pTexture)); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->words.w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)((((((TextureWidth*TextureHeight+3)>>2)-1)<2047?(((TextureWidth*TextureHeight+3)>>2)-1):2047)))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)((((1<<11)+(1>(TextureWidth/16)?1:(TextureWidth/16))-1)/(1>(TextureWidth/16)?1:(TextureWidth/16))))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((u32)(((((TextureWidth>>1)+7)>>3)))) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->words.w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)(((TextureWidth-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)(((TextureHeight-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; }
    { Gfx *g=D_800D8E24++; { Gfx *_gbi = (Gfx *)(g); _gbi->words.w0 = ((((unsigned int)(0xE4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(xh) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(yh) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->words.w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(xl) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(yl) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = ((unsigned int)(0)); }; { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = ((unsigned int)((((u32)((dsdx)) & 0xFFFF) << ((16)))|(((u32)((dtdy)) & 0xFFFF) << ((0))))); }; }
}
