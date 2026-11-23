#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "globals/collision.h"


void DamageEvent_Set(DamageEvent *event, CGameObjectInstance *source, s32 damage, f32 radius, s32 flags, void *shield);
void CAI__CallHurt(CGameObjectInstance *actor, void *ai, DamageEvent *event);
void CAI__SetMode(CGameObjectInstance *self, CAI *ai, s32 mode);

void EnemySwim_Collision(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;

    if (gCollisionResults.m_pWallRegion != 0) {
        ai->m_vLeashPos = ai->m_vInitialPos;
    }
    if (gCollisionResults.m_pInstance != NULL && ai->m_Mode == 0x10 && gCollisionResults.m_pInstance == ai->m_pPathTarget) {
        DamageEvent_Set(&event, self, 0x200, 25.599998f, 0x100000, NULL);
        CAI__CallHurt(gCollisionResults.m_pInstance, ((u8 *)&gCollisionResults.m_pInstance->m_AI), &event);
        CAI__SetMode(self, ai, 6);
    }
}
