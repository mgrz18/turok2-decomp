/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_6.CPP, func_8024A7A0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"

/* As func_0021D7BC, drawing through func_0020A33C and without setting the
 * actor's mask; clears D_800AB8B0 afterwards. */

typedef struct {
    s32 draw;               /* 0x00 */
    s32 shadow;             /* 0x04 */
    u8 pad8[0x4];
    s32 model;              /* 0x0C */
    u8 pad10[0x4];
    s32 index;              /* 0x14 */
    s32 set0;               /* 0x18 */
    s32 set1;               /* 0x1C */
} DrawParams;


extern s32 D_800B6D1C;      /* current view */
extern s32 D_800AB8B0;

void func_0021DC84(CGameObjectInstance *, s32, DrawParams *);
void func_0020A33C(s32, s32, s32, void *, s32, s32, s32);

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

void func_0021D928(CGameObjectInstance *actor, s32 arg1, DrawParams *params) {
    pickSets(actor, params->index, &params->set0, &params->set1);
    if (params->shadow != 0) {
        func_0021DC84(actor, arg1, params);
    }
    if (params->draw != 0) {
        func_0020A33C(params->model, ((s32)actor->m_pmtDrawMtxs), 1, actor->m_Lights[D_800B6D1C], 0, params->set0, actor->m_AI.m_nTexture);
    }
    D_800AB8B0 = 0;
}
