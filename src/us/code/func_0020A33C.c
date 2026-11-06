/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/CODE_8026D844.CPP, func_8026DA4C_de, with Turok 2's layouts. */

#include "common.h"
#include "gfx.h"
#include "structs/frame.h"


#define GFX_W(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (a); _g->w1 = (b); }

extern u32 gDisplayListSize;              /* command buffer length, in commands */

void *CISet__GetBlock(void *resource, s32 index);
void CCache__LockForDisplayList(s32 unused, void **resource);
s32 Render_SetupMaterial(void *material, s32 pass);

/* Draws one level of detail of a model resource when the frame's command buffer still has 3000
 * commands free: loads the matrix (or sets it as segment 1), sets segment 2 to the given texture
 * base or the model's own, loads an optional two-light block, then emits the display list of each
 * of the level's parts whose material Render_SetupMaterial accepts for the pass. */
void Render_DrawModel(void **resource, void *matrix, s32 segment, char *lights, void *textures, s32 lod, s32 pass) {
    void *header;
    void **parts;
    void **lodParts;
    void *part;
    void *base;
    s32 count;
    s32 i;

    if (gDisplayListSize - ((u32)gDList - (u32)gEngineApp_m_pCurrentFrameData->m_pDisplayList) / sizeof(Gfx) < 3000) {
        return;
    }
    header = *resource;
    parts = CISet__GetBlock(header, 2);
    if (lod < 0 || lod >= *(s32 *)parts) {
        return;
    }
    CCache__LockForDisplayList(0, resource);
    if (segment != 0) {
        GFX_W(gDList++, 0xDB060004, (u32)matrix);   /* gSPSegment(1, matrix) */
    } else {
        GFX_W(gDList++, 0xDA380003, (u32)matrix);   /* gSPMatrix(matrix, G_MTX_LOAD) */
    }
    base = textures;
    if (base == 0) {
        base = CISet__GetBlock(header, 0);
    }
    GFX_W(gDList++, 0xDB060008, (u32)base);     /* gSPSegment(2, base) */
    if (lights != 0) {
        GFX_W(gDList++, 0xDB020000, 0x18);           /* gSPNumLights(1) */
        GFX_W(gDList++, 0xDC08060A, (u32)(lights + 8)); /* gSPLight(lights + 8, 1) */
        GFX_W(gDList++, 0xDC08090A, (u32)lights);        /* gSPLight(lights, 2): ambient */
    }
    lodParts = CISet__GetBlock(parts, lod);
    count = *(s32 *)lodParts;
    for (i = 0; i < count; i++) {
        part = CISet__GetBlock(lodParts, i);
        if (Render_SetupMaterial(CISet__GetBlock(part, 0), pass) != 0) {
            void *list = CISet__GetBlock(part, 1);

            GFX_W(gDList++, 0xDE000000, (u32)list);     /* gSPDisplayList */
        }
    }
}
