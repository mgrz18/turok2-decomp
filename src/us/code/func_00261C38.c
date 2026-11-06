#include "common.h"
#include "structs/game_object.h"

f32 Random_Blend(f32 lo, f32 hi);
void EnemySwim_Collision(CGameObjectInstance *self, CAI *ai);

void EnemySwim_Setup(CGameObjectInstance *self, CAI *ai) {
    ai->m_pfCollision = (void (*)(CGameObjectInstance *, void *)) EnemySwim_Collision;
    ai->m_ModeTime = Random_Blend(0.0f, 62.83186f);
    ai->m_Enemy.m_Pitch = 0.0f;
    ai->m_Enemy.m_TurnRate = 0.017453294f;
    ai->m_Enemy.m_BaseScale = self->m_vScale.x;
    ai->m_Enemy.m_WanderTimer = 0.0f;
}
