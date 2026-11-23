#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "globals/collision.h"


void DamageEvent_Set(DamageEvent *event, CGameObjectInstance *source, s32 damage, f32 radius, s32 flags, void *shield);
void CAI__CallHurt(CGameObjectInstance *actor, void *ai, DamageEvent *event);
void CAI__SetMode(CGameObjectInstance *self, CAI *ai, s32 mode);

void EnemyRam_Collision(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;
    CCollisionResults *ps;

    if (ai->m_Health != 0 && ai->m_Mode == 0xE) {
        ps = &gCollisionResults;
        if (ps->m_pInstance != NULL && (ps->m_pInstance == ai->m_pSightTarget || ps->m_pInstance == ai->m_pPathTarget)) {
            DamageEvent_Set(&event, self, 0x500, 25.599998f, 0x100000, NULL);
            CAI__CallHurt(ps->m_pInstance, ((u8 *)&ps->m_pInstance->m_AI), &event);
            CAI__SetMode(self, ai, 4);
        }
    }
}
