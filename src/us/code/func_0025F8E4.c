#include "common.h"
#include "structs/game_object.h"

/* Raises the aggression by 25, up to 255. */
void EnemyMode_ReactionExplosion_Enter(s32 arg0, CAI *arg1) {
    s32 v = arg1->m_Aggression;
    v += 0x19;
    if (v >= 0x100) {
        v = 0xFF;
    }
    arg1->m_Aggression = v;
}
