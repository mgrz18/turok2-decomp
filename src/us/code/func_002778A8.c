/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/GFX/TEXT/802A8A94.CPP, func_802A7DE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef union Gfx {
    struct { u32 w0; u32 w1; } words;
    u64 force_structure_alignment;
} Gfx;
extern void func_002778A8(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern Gfx *D_800D8E24;
static __inline__ void rectangle(s32 x0,s32 y0,s32 x1,s32 y1,s32 r,s32 g,s32 b,s32 a) {
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->words.w1 = (0); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xFA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((((unsigned int)(r) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(g) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(b) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(a) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(x1>0?x1:0) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(y1>0?y1:0) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->words.w1 = ((((unsigned int)(x0>0?x0:0) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(y0>0?y0:0) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
}
extern Gfx *D_800D8E24;
void func_002778A8(s32 x0,s32 y0,s32 x1,s32 y1,s32 border,s32 invert,s32 red,s32 green,s32 blue,s32 alpha) {
    u8 r=red,g=green,b=blue,a=alpha;
    s32 lightR,lightG,lightB,darkR,darkG,darkB;
    lightR=invert ? r>>1 : r<<1;
    lightG=invert ? g>>1 : g<<1;
    lightB=invert ? b>>1 : b<<1;
    darkR=invert ? r<<1 : r>>1;
    darkG=invert ? g<<1 : g>>1;
    darkB=invert ? b<<1 : b>>1;
    rectangle(x0,y0,x1,y1,r,g,b,a);
    rectangle(x0,y0,x1,y0+border,lightR,lightG,lightB,a);
    rectangle(x1-border,y0,x1,y1,lightR,lightG,lightB,a);
    rectangle(x0,y1-border,x1,y1,darkR,darkG,darkB,a);
    rectangle(x0,y0,x0+border,y1,darkR,darkG,darkB,a);
}
