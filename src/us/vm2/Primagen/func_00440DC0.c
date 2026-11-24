/* The second VM module's copy of src/us/vm1/Primagen/func_00407DC0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/light.h"
#include "structs/model.h"
#include "globals/boss.h"
#include "globals/engine.h"
#include "protos/game.h"
#include "structs/twin_primagen.h"



s32 Cinema_PathPlaying(void);
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void ScriptAnimator_Apply(Slot00407DC0 *slot);
void ScreenTint_UpdateMode(void);

static __inline__ s32 hasBit(s32 mask, s32 bit) {
    return mask & bit;
}

/* Draws each of the boss's 8 parts present in its mask; part 1 shows mesh 8 in state 29. */
void Primagen_Draw_2(u8 *actor, u8 *state, CDrawArgs *req) {
    CGameObjectInstance *boss = gLevelScript.actor;
    s32 mask;
    s32 i;

    mask = boss->m_AI.m_VisBits;
    for (i = 0; i < 8; i++) {
        if (hasBit(mask, 1 << i)) {
            if (i == 1 && boss->m_AI.m_Mode == 29) {
                req->m_nModel = 8;
                req->m_nShadowModel = 8;
            } else {
                req->m_nModel = i;
                req->m_nShadowModel = i;
            }
            CGameObjectInstance__DrawShadow(actor, state, req);
            if (!Cinema_PathPlaying()) {
                ScriptAnimator_Apply(&gLevelScript.anims[i]);
                ScreenTint_UpdateMode();
            }
            /* the actor's draw matrices, lights and texture by member (N64 0x98, 0x110, 0x151) */
            Render_DrawModel(req->m_hModel, ((CGameObjectInstance *) actor)->m_pmtDrawMtxs, 1,
                             (char *) &((CGameObjectInstance *) actor)->m_Lights[gEvenOdd], 0, req->m_nModel,
                             ((CGameObjectInstance *) actor)->m_AI.m_nTexture);
            gBlendState = 0;
        }
    }
}
