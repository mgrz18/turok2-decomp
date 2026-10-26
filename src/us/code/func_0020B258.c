/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/8026AC38_4.CPP, func_8026C978_de, with Turok 2's layouts. */

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
extern char D_80110064[];           /* shared material */

void *func_002017D4(void *resource, s32 index);
void func_0020565C(s32 unused, void **resource);
s32 func_00208810(void *material, s32 pass);
void func_00207A70(s32 blend);

/* Draws one level of detail of a model resource with the shared material D_80110064, when the
 * frame's command buffer still has 3000 commands free and func_00208810 accepts the material for
 * the pass: sets the render mode, selects blend 2, loads the matrix (or sets it as segment 1),
 * loads an optional two-light block, registers the resource, sets segment 2 to the given texture
 * base or the model's own and emits each part's display list. */
void func_0020B258(void **resource, void *matrix, s32 segment, char *lights, void *textures, s32 lod, s32 pass) {
    void *header;
    void *base;
    void **lodParts;
    void *part;
    void *list;
    s32 count;
    s32 i;

    if (D_800C2214 - ((u32)D_800D8E24 - (u32)D_800F7070->commands) / sizeof(Gfx) < 3000) {
        return;
    }
    if (func_00208810(D_80110064, pass) == 0) {
        return;
    }
    GFX_W(D_800D8E24++, 0xE200001C, 0x0C184F50);    /* gDPSetRenderMode */
    func_00207A70(2);
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
    func_0020565C(0, resource);
    header = *resource;
    base = textures;
    if (base == 0) {
        base = func_002017D4(header, 0);
    }
    GFX_W(D_800D8E24++, 0xDB060008, (u32)base);     /* gSPSegment(2, base) */
    lodParts = func_002017D4(func_002017D4(header, 2), lod);
    count = *(s32 *)lodParts;
    for (i = 0; i < count; i++) {
        part = func_002017D4(lodParts, i);
        func_002017D4(part, 0);
        list = func_002017D4(part, 1);
        GFX_W(D_800D8E24++, 0xDE000000, (u32)list);     /* gSPDisplayList */
    }
}
