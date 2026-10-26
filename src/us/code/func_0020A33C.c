/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/CODE_8026D844.CPP, func_8026DA4C_de, with Turok 2's layouts. */

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

void *func_002017D4(void *resource, s32 index);
void func_0020565C(s32 unused, void **resource);
s32 func_00208810(void *material, s32 pass);

/* Draws one level of detail of a model resource when the frame's command buffer still has 3000
 * commands free: loads the matrix (or sets it as segment 1), sets segment 2 to the given texture
 * base or the model's own, loads an optional two-light block, then emits the display list of each
 * of the level's parts whose material func_00208810 accepts for the pass. */
void func_0020A33C(void **resource, void *matrix, s32 segment, char *lights, void *textures, s32 lod, s32 pass) {
    void *header;
    void **parts;
    void **lodParts;
    void *part;
    void *base;
    s32 count;
    s32 i;

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
    base = textures;
    if (base == 0) {
        base = func_002017D4(header, 0);
    }
    GFX_W(D_800D8E24++, 0xDB060008, (u32)base);     /* gSPSegment(2, base) */
    if (lights != 0) {
        GFX_W(D_800D8E24++, 0xDB020000, 0x18);           /* gSPNumLights(1) */
        GFX_W(D_800D8E24++, 0xDC08060A, (u32)(lights + 8)); /* gSPLight(lights + 8, 1) */
        GFX_W(D_800D8E24++, 0xDC08090A, (u32)lights);        /* gSPLight(lights, 2): ambient */
    }
    lodParts = func_002017D4(parts, lod);
    count = *(s32 *)lodParts;
    for (i = 0; i < count; i++) {
        part = func_002017D4(lodParts, i);
        if (func_00208810(func_002017D4(part, 0), pass) != 0) {
            void *list = func_002017D4(part, 1);

            GFX_W(D_800D8E24++, 0xDE000000, (u32)list);     /* gSPDisplayList */
        }
    }
}
