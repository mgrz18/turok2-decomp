/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A577C.c, func_002A577C, with Turok 2's layouts. */

#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "structs/swoosh.h"


typedef Swoosh Trail;
typedef CSwooshSystem TrailMgr;

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
    for (e = mgr->m_SwooshPool.used.head; e != NULL; e = e->next) {
        if (e->layer < 0) {
            if (e->flags & 8) {
                Trail_PlaceNodes(e);
            }
            if (e->flags & 4) {
                Trail_ComputeFrames(e, arg);
            }
            if (e->nodes.count >= 2) {
                if (e->layer >= 0) {
                    Trail_DrawTextured(mgr, e, arg);
                } else {
                    Trail_DrawUntextured(mgr, e, arg);
                }
            }
        }
    }
    for (e = mgr->m_SwooshPool.used.head; e != NULL; e = e->next) {
        if (e->layer > 0) {
            if (e->flags & 8) {
                Trail_PlaceNodes(e);
            }
            if (e->flags & 4) {
                Trail_ComputeFrames(e, arg);
            }
            if (e->nodes.count >= 2) {
                if (e->layer >= 0) {
                    Trail_DrawTextured(mgr, e, arg);
                } else {
                    Trail_DrawUntextured(mgr, e, arg);
                }
            }
        }
    }
}
