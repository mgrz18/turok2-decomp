#include "common.h"
#include "structs/game_object.h"

s32 func_00267E74(void);
f32 func_00268534(CGameObjectInstance *);
f32 func_002119FC(f32, f32);
f32 func_002974C0(f32);
f32 func_002982F0(f32);
void func_00239F00(CGameObjectInstance *, CVector3, s32);

void func_0025F504(CGameObjectInstance *self, CAI *ai) {
    f32 radius;
    f32 dist;
    f32 angle;
    CVector3 pos;

    if (func_00267E74()) {
        radius = func_00268534(self);
    } else {
        radius = 2048.0f;
    }
    dist = func_002119FC(0.0f, radius);
    angle = func_002119FC(-3.141593f, 3.141593f);
    pos.x = ai->m_vLeashPos.x + dist * func_002974C0(angle);
    pos.y = ai->m_vLeashPos.y;
    pos.z = ai->m_vLeashPos.z + dist * func_002982F0(angle);
    func_00239F00(self, pos, (s32) ai->m_pModeCurrent->m_pData);
    ai->m_vDesiredPos = self->header.m_vPos;
}
