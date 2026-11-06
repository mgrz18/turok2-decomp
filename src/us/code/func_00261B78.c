#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

extern CCollisionResults D_800F1CE0;

void DamageEvent_Set(DamageEvent *event, CGameObjectInstance *source, s32 damage, f32 radius, s32 flags, void *shield);
void CAI__CallHurt(CGameObjectInstance *actor, void *ai, DamageEvent *event);
void CAI__SetMode(CGameObjectInstance *self, CAI *ai, s32 mode);

void EnemySwim_Collision(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;

    if (D_800F1CE0.m_pWallRegion != 0) {
        ai->m_vLeashPos = ai->m_vInitialPos;
    }
    if (D_800F1CE0.m_pInstance != NULL && ai->m_Mode == 0x10 && D_800F1CE0.m_pInstance == ai->m_pPathTarget) {
        DamageEvent_Set(&event, self, 0x200, 25.599998f, 0x100000, NULL);
        CAI__CallHurt(D_800F1CE0.m_pInstance, ((u8 *)&D_800F1CE0.m_pInstance->m_AI), &event);
        CAI__SetMode(self, ai, 6);
    }
}
