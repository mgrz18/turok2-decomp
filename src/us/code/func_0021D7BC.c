/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_6.CPP, func_8024A7A0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/model.h"

/* Draw an actor's model (params->m_hModel) with the current view's lighting:
 * first pick its two colour sets (params->m_nModel/set1), from its intelligence's
 * death models (by AI mode) for an enemy (type 1), then its shadow (CGameObjectInstance__DrawShadow). */

extern s32 gEvenOdd;      /* current view */

void CGameObjectInstance__DrawShadow(CGameObjectInstance *, s32, CDrawArgs *);
void Render_QueueModelInstance(s32, s32, s32, void *, s32, s32, s32);

static __inline__ void pickSets(CGameObjectInstance *actor, s32 index, s32 *set0, s32 *set1) {
    CIntelligence *intel = actor->header.m_pIntelligence;

    if (intel->m_Type == 1) {
        s32 state;

        if (actor->m_AI.m_Mode == 0x3C) {
            state = actor->m_AI.m_ModeBefore;
        } else {
            state = actor->m_AI.m_Mode;
        }
        switch (state) {
            case 0x36:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[0];
                break;
            case 0x37:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[1];
                break;
            case 0x38:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[2];
                break;
            case 0x39:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[3];
                break;
            case 0x3a:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[4];
                break;
            case 0x3b:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[5];
                break;
            default:
                *set0 = (&intel->m_nModel)[index];
                *set1 = (&intel->m_nShadowModel)[index];
                break;
        }
    } else {
        *set0 = (&intel->m_nModel)[index];
        *set1 = (&intel->m_nShadowModel)[index];
    }
}

void CGameObjectInstance__DrawModel(CGameObjectInstance *actor, s32 arg1, CDrawArgs *params) {
    pickSets(actor, params->m_nVariant, &params->m_nModel, &params->m_nShadowModel);
    actor->m_AI.m_VisBits = 1 << params->m_nModel;
    if (params->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, arg1, params);
    }
    if (params->m_bDraw != 0) {
        Render_QueueModelInstance(params->m_hModel, ((s32)actor->m_pmtDrawMtxs), 1, actor->m_Lights[gEvenOdd], 0, params->m_nModel, actor->m_AI.m_nTexture);
    }
}
