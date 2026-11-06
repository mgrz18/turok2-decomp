/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER_4/8026AC38_3.CPP, func_8026C6D8_de, with Turok 2's layouts. */

#include "common.h"
#include "gfx.h"
#include "structs/frame.h"
#include "globals/render.h"


#define GFX_W(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (a); _g->w1 = (b); }

extern u32 gDisplayListSize;              /* command buffer length, in commands */
extern Gfx *gDList;             /* display list head */

void *CISet__GetBlock(void *resource, s32 index);
void CCache__LockForDisplayList(s32 unused, void **resource);

/* Draws one level of detail of a model resource flat-shaded in the primitive colour with the given
 * alpha, when the frame's command buffer still has 3000 commands free: loads the matrix (or sets it
 * as segment 1), loads an optional two-light block, sets segment 2 to the given texture base or the
 * model's own, sets up the combiner and render state, then emits every part's display list. The
 * cached render state gCurrentCombineMode/gCurrentRenderMode is invalidated before and after. */
void Render_DrawModelFlat(u8 alpha, void **resource, void *matrix, s32 segment, char *lights, void *textures, s32 lod) {
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
    if (lights != 0) {
        GFX_W(gDList++, 0xDB020000, 0x18);           /* gSPNumLights(1) */
        GFX_W(gDList++, 0xDC08060A, (u32)(lights + 8)); /* gSPLight(lights + 8, 1) */
        GFX_W(gDList++, 0xDC08090A, (u32)lights);        /* gSPLight(lights, 2): ambient */
    }
    base = textures;
    if (base == 0) {
        base = CISet__GetBlock(header, 0);
    }
    GFX_W(gDList++, 0xDB060008, (u32)base);     /* gSPSegment(2, base) */
    lodParts = CISet__GetBlock(parts, lod);
    sync = gDList++;
    gCurrentCombineMode = -1;
    gCurrentRenderMode = -1;
    geom = gFogGeometryMode | 0x20000;                    /* G_LIGHTING */
    count = *(s32 *)lodParts;
    GFX_W(sync, 0xE7000000, 0);                     /* gDPPipeSync */
    GFX_W(gDList++, 0xE3001001, 0);             /* gDPSetTextureLUT(G_TT_NONE) */
    GFX_W(gDList++, 0xD7FF0002, 0x80008000);    /* gSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF) */
    GFX_W(gDList++, 0xFA000000, alpha);         /* gDPSetPrimColor(0, 0, 0, 0, 0, alpha) */
    GFX_W(gDList++, 0xFCFFFFFF, 0xFFFDF6FB);    /* gDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE) */
    GFX_W(gDList++, 0xE200001C, gFogRenderModeShadeA | 0x104E50); /* gDPSetRenderMode */
    GFX_W(gDList++, 0xD9D8FBFB, 0);             /* gSPClearGeometryMode */
    geom |= 0x200004;                               /* G_SHADE | G_SHADING_SMOOTH */
    geom |= 0x400;                                  /* G_CULL_BACK */
    GFX_W(gDList++, 0xD9FFFFFF, geom); /* gSPSetGeometryMode */
    for (i = 0; i < count; i++) {
        part = CISet__GetBlock(lodParts, i);
        CISet__GetBlock(part, 0);
        list = CISet__GetBlock(part, 1);
        GFX_W(gDList++, 0xDE000000, (u32)list);     /* gSPDisplayList */
    }
    gCurrentRenderMode = -1;
    gCurrentCombineMode = -1;
}
