/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/8026AC38_3.CPP, func_8026C6D8_de, with Turok 2's layouts. */

#include "common.h"

typedef struct Gfx {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct GfxTask {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ Gfx *commands;
} GfxTask;

#define GFX_W(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (a); _g->w1 = (b); }

extern u32 D_800C2214;              /* command buffer length, in commands */
extern Gfx *D_800D8E24;             /* display list head */
extern GfxTask *D_800F7070;
extern s32 D_800AB880;
extern s32 D_800AB884;
extern u32 D_800AB8A4;              /* extra geometry mode bits */
extern u32 D_800AB8AC;              /* extra render mode bits */

void *func_002017D4(void *resource, s32 index);
void func_0020565C(s32 unused, void **resource);

/* Draws one level of detail of a model resource flat-shaded in the primitive colour with the given
 * alpha, when the frame's command buffer still has 3000 commands free: loads the matrix (or sets it
 * as segment 1), loads an optional two-light block, sets segment 2 to the given texture base or the
 * model's own, sets up the combiner and render state, then emits every part's display list. The
 * cached render state D_800AB880/D_800AB884 is invalidated before and after. */
void func_0020AF08(u8 alpha, void **resource, void *matrix, s32 segment, char *lights, void *textures, s32 lod) {
    void *header;
    void **parts;
    void **lodParts;
    void *part;
    void *list;
    void *base;
    s32 count;
    s32 i;
    u32 geom;
    Gfx *sync;

    if (D_800C2214 - ((u32)D_800D8E24 - (u32)D_800F7070->commands) / sizeof(Gfx) < 3000) {
        return;
    }
    header = *resource;
    parts = func_002017D4(header, 2);
    if (lod < 0 || lod >= *(s32 *)parts) {
        return;
    }
    func_0020565C(0, resource);
    if (segment != 0) {
        GFX_W(D_800D8E24++, 0xDB060004, (u32)matrix);   /* gSPSegment(1, matrix) */
    } else {
        GFX_W(D_800D8E24++, 0xDA380003, (u32)matrix);   /* gSPMatrix(matrix, G_MTX_LOAD) */
    }
    if (lights != 0) {
        GFX_W(D_800D8E24++, 0xDB020000, 0x18);           /* gSPNumLights(1) */
        GFX_W(D_800D8E24++, 0xDC08060A, (u32)(lights + 8)); /* gSPLight(lights + 8, 1) */
        GFX_W(D_800D8E24++, 0xDC08090A, (u32)lights);        /* gSPLight(lights, 2): ambient */
    }
    base = textures;
    if (base == 0) {
        base = func_002017D4(header, 0);
    }
    GFX_W(D_800D8E24++, 0xDB060008, (u32)base);     /* gSPSegment(2, base) */
    lodParts = func_002017D4(parts, lod);
    sync = D_800D8E24++;
    D_800AB880 = -1;
    D_800AB884 = -1;
    geom = D_800AB8A4 | 0x20000;                    /* G_LIGHTING */
    count = *(s32 *)lodParts;
    GFX_W(sync, 0xE7000000, 0);                     /* gDPPipeSync */
    GFX_W(D_800D8E24++, 0xE3001001, 0);             /* gDPSetTextureLUT(G_TT_NONE) */
    GFX_W(D_800D8E24++, 0xD7FF0002, 0x80008000);    /* gSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF) */
    GFX_W(D_800D8E24++, 0xFA000000, alpha);         /* gDPSetPrimColor(0, 0, 0, 0, 0, alpha) */
    GFX_W(D_800D8E24++, 0xFCFFFFFF, 0xFFFDF6FB);    /* gDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE) */
    GFX_W(D_800D8E24++, 0xE200001C, D_800AB8AC | 0x104E50); /* gDPSetRenderMode */
    GFX_W(D_800D8E24++, 0xD9D8FBFB, 0);             /* gSPClearGeometryMode */
    geom |= 0x200004;                               /* G_SHADE | G_SHADING_SMOOTH */
    geom |= 0x400;                                  /* G_CULL_BACK */
    GFX_W(D_800D8E24++, 0xD9FFFFFF, geom); /* gSPSetGeometryMode */
    for (i = 0; i < count; i++) {
        part = func_002017D4(lodParts, i);
        func_002017D4(part, 0);
        list = func_002017D4(part, 1);
        GFX_W(D_800D8E24++, 0xDE000000, (u32)list);     /* gSPDisplayList */
    }
    D_800AB884 = -1;
    D_800AB880 = -1;
}
