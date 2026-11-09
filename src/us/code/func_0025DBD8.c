#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "protos/ai.h"

/* AI combat update: turns toward the target, then picks a reaction (taunt,
 * the 0x11F4-range anims, a dodge or a special melee for type 0x259). */

extern void AI_UpdateTargets(CGameObjectInstance *actor, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void EnemyMode_Move_Update(CGameObjectInstance *actor, CAI *ai);
extern s32 EnemyAI_PickAttack(CGameObjectInstance *actor, CAI *ai, CAITargets *t);
extern void EnemyAI_TryDodge(CGameObjectInstance *actor, CAI *ai);

static __inline__ s8 canTaunt(CAI *ai, CEnemyIntelligence *info) {
    if (ai->m_Enemy.m_Behavior[0] && (info->m_wComboEndAnimFlags & 0x1F)) {
        return 1;
    }
    if (ai->m_Enemy.m_Behavior[1] && (info->m_wComboEndAnimFlags & 0x3E0)) {
        return 1;
    }
    if (ai->m_Enemy.m_Behavior[2] && (info->m_wComboEndAnimFlags & 0x7C00)) {
        return 1;
    }
    return 0;
}

void EnemyMode_Attack_Update(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    f32 angle;
    f32 skill;

    AI_UpdateTargets(actor, ai, &t, 0, 1);
    angle = 0.261799425f;
    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        angle = 1.0471977f;
    }
    AI_TurnClamped(actor, ai, angle, t.m_Sight.m_Angle);
    if ((ai->m_dwFlags & 0x4000000) && canTaunt(ai, &actor->header.m_pIntelligence->m_Enemy)) {
        CAI__SetMode(actor, ai, 0x13);
        ai->m_dwFlags |= 0x4000000;
        ai->m_ModeLastFrame = ai->m_Mode;
        ai->m_AnimModeSpeed = ai->m_pModeCurrent->m_AnimSpeed;
    } else if (ai->m_AnimType >= 4500 && ai->m_AnimType < 4555) {
        skill = ai->m_Aggression;
        skill = skill * (1.0f / 255);
        skill = skill * 5.0f + 1.0f; /* hits needed: 1..6 */
        if (ai->m_AnimRepeatedCount >= (s32) skill) {
            CAI__SetMode(actor, ai, 0xA);
        }
    } else if (ai->m_AnimRepeatedCount != 0) {
        EnemyMode_Move_Update(actor, ai);
    }
    if (actor->m_nTypeFlag == 0x259 && EnemyAI_PickAttack(actor, ai, &t) == 0xE) {
        CAI__SetMode(actor, ai, 0xE);
    }
    EnemyAI_TryDodge(actor, ai);
}
