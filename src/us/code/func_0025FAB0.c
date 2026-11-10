#include "common.h"
#include "structs/game_object.h"

/* Raises the aggression by 10, up to 255, and clears the condition timer. */
void EnemyMode_ReactionShocked_Enter(sptr arg0, CAI *arg1) {
    s32 v = arg1->m_Aggression;
    v += 0xA;
    if (v >= 0x100) {
        v = 0xFF;
    }
    arg1->m_Aggression = v;
    arg1->m_ConditionTimer = 0;
}
