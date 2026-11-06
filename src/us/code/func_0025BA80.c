#include "common.h"
#include "structs/game_object.h"
#include "structs/model.h"

/* AI draw callback: draws the actor faded in or out (dying / teleporting states). */

extern s32 gBlendState; /* fade mode */
extern f32 gBlendFade[2]; /* fade level, alpha */
extern s32 gEvenOdd; /* current view */
extern u8 D_801100D4[];

extern void CGameObjectInstance__DrawModelImmediate(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params);
extern void CGameObjectInstance__DrawModel(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params);
extern void Render_DrawModelShared(s32 model, s32 matrix, s32 segment, void *lights, s32 textures, s32 lod, s32 pass);
extern void func_0020BBE4(void *arg0, s32 renderMode, s32 model, s32 matrix, s32 projected, void *lights,
                          s32 segment, s32 group, s32 arg8);

void EnemyAI_DrawFaded(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params) {
    u8 alpha;

    if (ai->m_Mode == 0x20) {
        CGameObjectInstance__DrawModelImmediate(actor, ai, params);
        Render_DrawModelShared(params->m_hModel, actor->m_pmtDrawMtxs, 1, actor->m_Lights[gEvenOdd], 0, ((s8 *)params)[0x1B], -1);
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
            gBlendFade[0] = 255 - alpha;
            CGameObjectInstance__DrawModelImmediate(actor, ai, params);
        }
        if (alpha != 0) {
            gBlendFade[0] = alpha;
            gBlendState = 1;
            func_0020BBE4(D_801100D4, -1, params->m_hModel, actor->m_pmtDrawMtxs, 1, actor->m_Lights[gEvenOdd], 0,
                          params->m_nModel, actor->m_AI.m_nTexture);
        }
        gBlendState = 0;
    } else {
        CGameObjectInstance__DrawModel(actor, ai, params);
    }
}
