#include "common.h"
#include "structs/game_object.h"

/* AI callback: in modes 0x11-0x12 sets 0x120 from the intensity (0..255 ->
 * 60..15); clears AI flag 0x4000000. */
void EnemyMode_Attack_Enter(CGameObjectInstance *self, CAI *ai) {
    f32 level;

    if (ai->m_Mode == 0x11 || ai->m_Mode == 0x12) {
        level = ai->m_Aggression;
        level *= (1.0f / 255);
        level = level * -45.0f + 60.0f;
        ai->m_Enemy.m_AttackDelay = level;
    }
    ai->m_dwFlags &= ~0x4000000;
}
