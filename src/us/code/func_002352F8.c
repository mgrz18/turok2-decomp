/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A577C.c, func_002A577C, with Turok 2's layouts. */

#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"


typedef struct Trail Trail;
struct Trail {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Trail *next;
    /* 0x08 */ u8 pad8[0x2C];
    /* 0x34 */ s32 layer;
    /* 0x38 */ u8 pad38[4];
    /* 0x3C */ u32 flags;
    /* 0x40 */ u8 pad40[8];
    /* 0x48 */ s32 nodeCount;
};

typedef struct TrailMgr {
    /* 0x0000 */ u8 pad0[0x7520];
    /* 0x7520 */ Trail *trails;
} TrailMgr;

void Render_InvalidateModeCache(void);
void Render_SetRenderMode(s32);
void Render_SetCombineMode(s32);
void Texture_InvalidateCache(void);
void Trail_PlaceNodes(Trail *);
void Trail_ComputeFrames(Trail *, s32);
void Trail_DrawTextured(TrailMgr *, Trail *, s32);
void Trail_DrawUntextured(TrailMgr *, Trail *, s32);

/* Draws the trails: the negative layers first, then the positive ones. */
void Trail_DrawAll(TrailMgr *mgr, s32 arg) {
    Trail *e;

    GFX_CMD(0xE7000000, 0)
    Render_InvalidateModeCache();
    Render_SetRenderMode(0xE);
    Render_SetCombineMode(0x1C);
    GFX_CMD(0xD9F8FB7F, 0)
    GFX_CMD(0xD9FFFFFF, 0x200004)
    GFX_CMD(0xE3000A01, 0)
    GFX_CMD(0xE3000C00, 0x80000)
    GFX_CMD(0xD7000000, 0)
    Texture_InvalidateCache();
    for (e = mgr->trails; e != NULL; e = e->next) {
        if (e->layer < 0) {
            if (e->flags & 8) {
                Trail_PlaceNodes(e);
            }
            if (e->flags & 4) {
                Trail_ComputeFrames(e, arg);
            }
            if (e->nodeCount >= 2) {
                if (e->layer >= 0) {
                    Trail_DrawTextured(mgr, e, arg);
                } else {
                    Trail_DrawUntextured(mgr, e, arg);
                }
            }
        }
    }
    for (e = mgr->trails; e != NULL; e = e->next) {
        if (e->layer > 0) {
            if (e->flags & 8) {
                Trail_PlaceNodes(e);
            }
            if (e->flags & 4) {
                Trail_ComputeFrames(e, arg);
            }
            if (e->nodeCount >= 2) {
                if (e->layer >= 0) {
                    Trail_DrawTextured(mgr, e, arg);
                } else {
                    Trail_DrawUntextured(mgr, e, arg);
                }
            }
        }
    }
}
