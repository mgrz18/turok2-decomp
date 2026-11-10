#include "common.h"
#include "structs/game_object.h"
#include "structs/model.h"
#include "globals/engine.h"
#include "structs/light.h"

/* AI draw callback: draws the actor faded in or out (dying / teleporting states). */

extern u8 D_801100D4[];

extern void CGameObjectInstance__DrawModelImmediate(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params);
extern void CGameObjectInstance__DrawModel(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params);
extern void Render_DrawModelShared(sptr model, sptr matrix, s32 segment, void *lights, sptr textures, s32 lod, s32 pass);
extern void func_0020BBE4(void *arg0, s32 renderMode, sptr model, s32 matrix, s32 projected, void *lights, sptr segment, s32 group, s32 arg8);

void EnemyAI_DrawFaded(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params) {
    u8 alpha;

    if (ai->m_Mode == 0x20) {
        CGameObjectInstance__DrawModelImmediate(actor, ai, params);
        Render_DrawModelShared(params->m_hModel, actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0, ((s8 *)params)[0x1B], -1);
        return;
    }
    if (ai->m_Enemy.m_Timer > 0.0f) {
        if (ai->m_Mode == 0x19) {
            alpha = ai->m_AnimRepeatedCount * 255 / 4;
        } else {
            alpha = 255;
            if (ai->m_Enemy.m_Timer < 30.0f) {
                alpha = ai->m_Enemy.m_Timer * 8.5f;
            }
        }
        if (alpha != 255) {
            gBlendState = 1;
            gBlendState_m_Color[3] = 255 - alpha;
            CGameObjectInstance__DrawModelImmediate(actor, ai, params);
        }
        if (alpha != 0) {
            gBlendState_m_Color[3] = alpha;
            gBlendState = 1;
            func_0020BBE4(D_801100D4, -1, params->m_hModel, actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0,
                          params->m_nModel, actor->m_AI.m_nTexture);
        }
        gBlendState = 0;
    } else {
        CGameObjectInstance__DrawModel(actor, ai, params);
    }
}
