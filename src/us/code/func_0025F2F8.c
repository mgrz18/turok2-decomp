#include "common.h"
#include "structs/game_object.h"

/* AI callback: clears AI flag 0x4000000 and actor flag 0x20000; sets the
 * timer from the intensity (0..255 -> 15..45). */
void EnemyMode_Combo_Enter(CGameObjectInstance *self, CAI *ai) {
    f32 level;

    ai->m_dwFlags &= ~0x4000000;
    self->m_dwFlags &= ~0x20000;
    level = ai->m_Aggression;
    level *= (1.0f / 255);
    level = level * 30.0f + 15.0f;
    ai->m_ConditionTimer = level;
}
