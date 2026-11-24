#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "protos/enemy_ai.h"

/* Index-2 attack follow-up: in the 7020-7041 animation band it chains to the
 * next move; otherwise it finishes the current one. */

s32 EnemyAnim_NormalDeath_B0390(void *, void *);
s32 EnemyAnim_NormalDeath_B03C0(void *, void *);

void EnemyMode_Death_Update(CGameObjectInstance *actor, CAI *state) {
    CEnemyIntelligence *info = &actor->header.m_pIntelligence->m_Enemy;
    s32 anim;

    if (state->m_Enemy.m_BehaviorIndex == 2) {
        switch (state->m_AnimType) {
        case 7020:
        case 7025:
        case 7030:
        case 7035:
        case 7040:
            if (state->m_AnimRepeatedCount == 0) {
                return;
            }
            anim = EnemyAnim_NormalDeath_B0390(actor, state);
            break;
        case 7021:
        case 7026:
        case 7031:
        case 7036:
        case 7041:
            if (!GameObject_IsOnGround(actor)) {
                return;
            }
            anim = EnemyAnim_NormalDeath_B03C0(actor, state);
            break;
        default:
            goto finish;
        }
        state->m_AnimIndex = CGameObjectInstance__PickRandomAnim(actor, anim, -1);
        state->m_AnimType = anim;
        state->m_AnimRepeatedCount = 0;
        state->m_dwFlags &= ~1;
        return;
    }
finish:
    if (state->m_AnimRepeatedCount != 0) {
        if (state->m_Mode == 0x35) {
            CAI__SetMode(actor, state, 0x36);
        } else {
            state->m_dwFlags |= 0x20;
            actor->m_dwFlags &= ~0x01000000;
            AI_DropPickupsOnce(actor, state, 4, info->m_DropMask, info->m_DropMask2);
            actor->m_dwFlags &= ~0x20000;
            if (state->m_ModeTime > 90.0f) {
                CAI__SetMode(actor, state, 0x3C);
            }
        }
        if (info->m_dwFlags & 0x10000) {
            actor->m_dwFlags &= ~0x100;
            actor->m_dwFlags &= ~0x2000;
            AI_StopSound(actor, state);
            if (actor->m_dwFlags & 0x80000) {
                CListedInstance__Free(actor);
            }
        }
    } else {
        state->m_ModeTime = 0.0f;
    }
}
