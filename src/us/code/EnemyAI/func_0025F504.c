#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"


/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS GameObject_IsLeashed receives EnemyMode_TeleportIn_Enter's own argument
 * registers (pass-through); the host passes them. */
void EnemyMode_TeleportIn_Enter(CGameObjectInstance *self, CAI *ai) {
    f32 radius;
    f32 dist;
    f32 angle;
    CVector3 pos;

    if (GameObject_IsLeashed(self)) {
        radius = GameObject_GetLeashRadius(self);
    } else {
        radius = 2048.0f;
    }
    dist = Random_Blend(0.0f, radius);
    angle = Random_Blend(-3.141593f, 3.141593f);
    pos.x = ai->m_vLeashPos.x + dist * func_002974C0(angle);
    pos.y = ai->m_vLeashPos.y;
    pos.z = ai->m_vLeashPos.z + dist * func_002982F0(angle);
    Collision_MoveInstance(self, pos, (sptr) ai->m_pModeCurrent->m_pData);
    ai->m_vDesiredPos = self->header.m_vPos;
}
