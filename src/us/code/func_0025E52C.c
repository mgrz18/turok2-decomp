#include "common.h"
#include "globals/ai.h"

/* Pushes an AI state's alert level up when a noise of the given kind is
 * heard within 3072 units, remembering where it came from. */

typedef struct {
    /* 0x00 */ u8 pad00[0x44];
    /* 0x44 */ f32 alert;
    /* 0x48 */ u8 pad48[0xE8];
    /* 0x130 */ s8 modes[3];
    /* 0x133 */ s8 index;
} AlertState;

f32 CVector3__Distance(void *, void *);

#ifdef TARGET_PORT
#include "structs/game_object.h"

/* The port: the same on CAI's members. AlertState is the N64 layout of CAI: the behaviour table
 * and its index at 0x130/0x133 (m_Enemy.m_Behavior[], m_BehaviorIndex) are elsewhere on the host,
 * so a heard or seen player never raised m_ActionTime: enemies stayed unalerted (m_Action 0),
 * AI_FindBestTarget skipped them for path targets and they never chose a melee attack. */
void AI_HearNoise(u8 *actor_, CAI *ai, CVector3 *pos, s32 kind) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    s32 mode = ai->m_Enemy.m_Behavior[ai->m_Enemy.m_BehaviorIndex];
    f32 strength;
    f32 dist;
    f32 alert;

    if (mode >= 0) {
    if (mode < 3) {
        switch (kind) {
        case 1:
            strength = 45.0f;
            break;
        case 2:
            strength = 105.0f;
            break;
        case 3:
            strength = 150.0f;
            break;
        case 0:
        default:
            return;
        }
        dist = CVector3__Distance(&actor->header.m_vPos, pos);
        if (dist < 3072.0f) {
            strength = strength * (1.0f / 3072.0f) * (3072.0f - dist);
            alert = ai->m_ActionTime;
            if (alert < strength) {
                ai->m_ActionTime = alert + strength;
                if (strength < alert + strength) {
                    ai->m_ActionTime = strength;
                }
                gEnemyLastNoisePos = *pos;
            }
        }
    }
    }
}
#else
void AI_HearNoise(u8 *actor, AlertState *state, CVector3 *pos, s32 kind) {
    s32 mode = state->modes[state->index];
    f32 strength;
    f32 dist;
    f32 alert;

    if (mode >= 0) {
    if (mode < 3) {
        switch (kind) {
        case 1:
            strength = 45.0f;
            break;
        case 2:
            strength = 105.0f;
            break;
        case 3:
            strength = 150.0f;
            break;
        case 0:
        default:
            return;
        }
        dist = CVector3__Distance(actor + 4, pos);
        if (dist < 3072.0f) {
            strength = strength * (1.0f / 3072.0f) * (3072.0f - dist);
            alert = state->alert;
            if (alert < strength) {
                state->alert = alert + strength;
                if (strength < alert + strength) {
                    state->alert = strength;
                }
                gEnemyLastNoisePos = *pos;
            }
        }
    }
    }
}
#endif
