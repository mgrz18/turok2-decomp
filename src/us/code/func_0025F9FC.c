#include "common.h"
#include "structs/game_object.h"

/* AI reaction (same code as EnemyAI_OnHit case 10): raises the intensity by 10
 * (at most 255) and delays the next reaction by 105..60 frames. */
void EnemyMode_ReactionBlinded_Enter(void *actor, CAI *ai) {
    s32 v = ai->m_Aggression;
    f32 level;

    v += 10;
    if (v >= 0x100) {
        v = 0xFF;
    }
    ai->m_Aggression = v;
    level = ai->m_Aggression;
    level *= (1.0f / 255);
    level = level * -45.0f + 105.0f;
    ai->m_ConditionTimer += level;
}
