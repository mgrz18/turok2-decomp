/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/8026AC38_4.CPP, func_8026C978_de, with Turok 2's layouts. */

#include "common.h"
#include "gfx.h"
#include "structs/frame.h"
#include "globals/scene.h"
#include "globals/render.h"
#include "protos/iset.h"


#define GFX_W(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (a); _g->w1 = (b); }


void CCache__LockForDisplayList(s32 unused, void **resource);
s32 Render_SetupMaterial(void *material, s32 pass);
void Render_SetCombineMode(s32 blend);

/* Draws one level of detail of a model resource with the shared material gScene_m_Geom, when the
 * frame's command buffer still has 3000 commands free and Render_SetupMaterial accepts the material for
 * the pass: sets the render mode, selects blend 2, loads the matrix (or sets it as segment 1),
 * loads an optional two-light block, registers the resource, sets segment 2 to the given texture
 * base or the model's own and emits each part's display list. */
void Render_DrawModelShared(void **resource, void *matrix, s32 segment, char *lights, void *textures, s32 lod, s32 pass) {
    void *header;
    void *base;
    void **lodParts;
    void *part;
    void *list;
    s32 count;
    s32 i;

    if (gDisplayListSize - ((u32)gDList - (u32)gEngineApp_m_pCurrentFrameData->m_pDisplayList) / sizeof(Gfx) < 3000) {
        return;
    }
    if (Render_SetupMaterial(gScene_m_Geom, pass) == 0) {
        return;
    }
    GFX_W(gDList++, 0xE200001C, 0x0C184F50);    /* gDPSetRenderMode */
    Render_SetCombineMode(2);
    if (segment != 0) {
        GFX_W(gDList++, 0xDB060004, (u32)matrix);   /* gSPSegment(1, matrix) */
    } else {
        GFX_W(gDList++, 0xDA380003, (u32)matrix);   /* gSPMatrix(matrix, G_MTX_LOAD) */
    }
    if (lights != 0) {
        GFX_W(gDList++, 0xDB020000, 0x18);           /* gSPNumLights(1) */
        GFX_W(gDList++, 0xDC08060A, (u32)(lights + 8)); /* gSPLight(lights + 8, 1) */
        GFX_W(gDList++, 0xDC08090A, (u32)lights);        /* gSPLight(lights, 2): ambient */
    }
    CCache__LockForDisplayList(0, resource);
    header = *resource;
    base = textures;
    if (base == 0) {
        base = CISet__GetBlock(header, 0);
    }
    GFX_W(gDList++, 0xDB060008, (u32)base);     /* gSPSegment(2, base) */
    lodParts = CISet__GetBlock(CISet__GetBlock(header, 2), lod);
    count = *(s32 *)lodParts;
    for (i = 0; i < count; i++) {
        part = CISet__GetBlock(lodParts, i);
        CISet__GetBlock(part, 0);
        list = CISet__GetBlock(part, 1);
        GFX_W(gDList++, 0xDE000000, (u32)list);     /* gSPDisplayList */
    }
}
