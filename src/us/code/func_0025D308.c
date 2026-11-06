#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* AI dodge check: if the definition enables dodging for the current mode,
 * dodges with a probability that grows with the AI's skill (0..255), to the
 * side away from the threat (animation 8 or 9). */

extern s32 Random_Next(void);
extern s32 AI_IsAimedAt(CGameObjectInstance *actor, CAI *ai, s32 threat, f32 angle);
extern void CAI__SetMode(CGameObjectInstance *actor, CAI *ai, s32 anim);

void EnemyAI_TryDodge(CGameObjectInstance *actor, CAI *ai) {
    f32 skill;
    s32 chance;
    s32 side;
    s32 mode;
    s32 mode2;

    mode = ai->m_Enemy.m_BehaviorIndex;
    mode2 = mode;
    if ((mode == 0 && (actor->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags & 0x1F)) || (mode == 1 && (actor->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags & 0x3E0))
        || (mode2 == 2 && (actor->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags & 0x7C00))) {
        skill = ai->m_Aggression * (1.0f / 255);
        chance = skill * (skill * 600.0f) + 100.0f;
        if (Random_Next() % 10000 < chance) {
            side = AI_IsAimedAt(actor, ai, ai->m_pSightTarget, 0.261799425f);
            if (side == -1) {
                CAI__SetMode(actor, ai, 8);
            } else if (side == 1) {
                CAI__SetMode(actor, ai, 9);
            }
        }
    }
}
