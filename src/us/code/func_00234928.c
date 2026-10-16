/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_10.c, func_802A3DD4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef float f32;
typedef union Gfx {
    struct { u32 w0; u32 w1; } words;
    u64 force_structure_alignment;
} Gfx;
struct ObjRecAt28V1;
typedef struct ObjRecAt28V1 ObjRecAt28V1;
struct ObjRecAt28V1;
struct ObjRecAt28V1 {
    struct ObjRecAt28V1 *next;
    s32 field4;
    f32 unk8;
    char padC[0x28 - 0xC];
    s32 matrix[2][16];
};
struct Key;
struct Key {
    char pad0[0x14];
};
struct Key;
struct Key;
typedef struct Key Key;
struct Shade;
typedef struct Shade Shade;
struct Actor_func_802A274C_de;
typedef struct Actor_func_802A274C_de Actor_func_802A274C_de;
struct Morph;
typedef struct Morph Morph;
struct Shade;
struct Shade {
    u8 a;
    u8 b;
    u8 g;
    u8 r;
};
struct Blend;
struct Blend {
    Shade from;
    Shade to;
    u16 first;
    u16 second;
};
struct Track_func_802A274C_de;
struct Track_func_802A274C_de {
    char pad0[4];
    f32 span;
    f32 total;
    char padC[0x12 - 0xC];
    u8 closed;
    char pad13[0x21 - 0x13];
    u8 scaleS, scaleT, count;
    struct Blend blends[1];
};
struct Actor_func_802A274C_de;
struct Track_func_802A274C_de;
struct Actor_func_802A274C_de {
    char pad0[8];
    struct Track_func_802A274C_de *track;
    struct Key *first;
    struct Key *second;
    char pad14[0x24 - 0x14];
    f32 time;
    char pad28[0x3C - 0x28];
    u32 flags;
    s32 field40;
    ObjRecAt28V1 *clock;
};
struct Blend;
typedef struct Blend Blend;
struct func_802A66B4_S1;
typedef struct func_802A66B4_S1 func_802A66B4_S1;
struct Blend;
struct Key;
struct Morph;
struct Morph {
    s32 count;
    struct Blend *blends;
    struct Key *first;
    struct Key *second;
    union { void *firstFrame; f32 scaleS; };
    union { void *secondFrame; f32 scaleT; };
    f32 textureScaleS;
    f32 textureScaleT;
    f32 step;
};
struct Vtx;
typedef struct Vtx Vtx;
struct Vtx;
struct Vtx {
    s16 x;
    s16 y;
    s16 z;
    s16 flag;
    s16 s;
    s16 t;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};
struct func_802A66B4_S1;
struct func_802A66B4_S1 {
    char pad0[0x2588];
    s32 unk2588;
    char pad2588[0x258C - 0x2588 - sizeof(s32)];
    s32 unk258C;
};
extern Gfx *D_800D8E24;
extern s32 D_800B6D1C;
struct Track_func_802A274C_de;
typedef struct Track_func_802A274C_de Track_func_802A274C_de;
extern float D_800A6488;
Vtx *func_00235F8C(func_802A66B4_S1 *pool, s32 count);
void func_0023353C(Actor_func_802A274C_de *actor, ObjRecAt28V1 *clock, Vtx *vertices, Morph *morph);
void func_00234928(func_802A66B4_S1 *pool, Actor_func_802A274C_de *actor) {
    Morph storage;
    Morph *morph = &storage;
    ObjRecAt28V1 *clock, *next;
    Vtx *vertices;
    u32 count, i;
    s32 edges;
    morph->blends = actor->track->blends;
    morph->count = actor->track->count;
    if (actor->flags & 2) { morph->first = actor->first; morph->second = actor->second; }
    else { morph->first = actor->first; morph->second = actor->first; }
    morph->scaleS = actor->track->scaleS * D_800A6488;
    morph->scaleT = actor->track->scaleT * D_800A6488;
    clock = actor->clock;
    next = clock->next;
    count = actor->track->count;
    edges = count - 1;
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xDA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (5)))) << (19)) | (((unsigned int)(((((unsigned int)(0x1)) << 1)) ^ (((unsigned int)(0x1)) << 0)) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)(&clock->matrix[D_800B6D1C])); };
    while (clock != 0 && next != 0 && 0.0f < clock->unk8 && 0.0f < next->unk8) {
        vertices = func_00235F8C(pool, count);
        if (vertices == 0) break;
        func_0023353C(actor, clock, vertices, &storage);
        { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0x01) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(count) & (0xFFFFFFFFU >> (32 - (8)))) << (12)) | (((unsigned int)((0) + (count)) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->words.w1 = ((unsigned int)(vertices)); };
        { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0xDA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (5)))) << (19)) | (((unsigned int)(((((unsigned int)(0x1)) << 1)) ^ (((unsigned int)(0x1)) << 0)) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->words.w1 = ((unsigned int)(&next->matrix[D_800B6D1C])); };
        vertices = func_00235F8C(pool, count);
        if (vertices == 0) return;
        func_0023353C(actor, next, vertices, &storage);
        { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->words.w0 = ((((unsigned int)(0x01) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(count) & (0xFFFFFFFFU >> (32 - (8)))) << (12)) | (((unsigned int)((count) + (count)) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->words.w1 = ((unsigned int)(vertices)); };
        i = 0;
        while ((s32)i < edges) {
            { Gfx *cmd = D_800D8E24++;
              cmd->words.w0 = (((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | ((count + i + 1) * 2 & 255)) | (0x06 << 24);
              cmd->words.w1 = ((i * 2 & 255) << 16) | (((count + i + 1) * 2 & 255) << 8) | ((i + 1) * 2 & 255);
            }
            i++;
        }
        if (actor->track->closed) {
            { Gfx *cmd = D_800D8E24++;
              cmd->words.w0 = (((i * 2 & 255) << 16) | (((count + i) * 2 & 255) << 8) | (count * 2 & 255)) | (0x06 << 24);
              cmd->words.w1 = ((i * 2 & 255) << 16) | ((count * 2 & 255) << 8);
            }
        }
        clock = next;
        next = clock->next;
    }
}
