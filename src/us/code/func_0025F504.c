#include "common.h"
#include "structs/game_object.h"

s32 func_00267E74(void);
f32 GameObject_GetLeashRadius(CGameObjectInstance *);
f32 Random_Blend(f32, f32);
f32 func_002974C0(f32);
f32 func_002982F0(f32);
void Collision_MoveInstance(CGameObjectInstance *, CVector3, s32);

void EnemyMode_TeleportIn_Enter(CGameObjectInstance *self, CAI *ai) {
    f32 radius;
    f32 dist;
    f32 angle;
    CVector3 pos;

    if (func_00267E74()) {
        radius = GameObject_GetLeashRadius(self);
    } else {
        radius = 2048.0f;
    }
    dist = Random_Blend(0.0f, radius);
    angle = Random_Blend(-3.141593f, 3.141593f);
    pos.x = ai->m_vLeashPos.x + dist * func_002974C0(angle);
    pos.y = ai->m_vLeashPos.y;
    pos.z = ai->m_vLeashPos.z + dist * func_002982F0(angle);
    Collision_MoveInstance(self, pos, (s32) ai->m_pModeCurrent->m_pData);
    ai->m_vDesiredPos = self->header.m_vPos;
}
