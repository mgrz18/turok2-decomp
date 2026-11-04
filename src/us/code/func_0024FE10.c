#include "common.h"
#include "structs/intelligence.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ f32 yaw;
} BobObject;

void func_002117DC(f32 *value, f32 target, f32 rate);
f32 func_0021170C(f32 value, f32 rate);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);

/* Settles the view tilt and kicks, then bobs the view with the stride,
 * scaled by the speed. */
void func_0024FE10(CPlayer *p, BobObject *obj) {
    f32 speed;
    f32 amount;
    f32 phase;
    f32 base;
    f32 y;
    f32 side;

    func_002117DC(&p->m_vHeadRotOffset.z, 0.0f, 0.25f);
    func_002117DC(&p->m_vHeadRotOffset.y, 0.0f, 0.5f);
    p->m_vHeadWobbleRotOffset.x = func_0021170C(p->m_vHeadWobbleRotOffset.x, 0.087266475f);
    p->m_vHeadWobbleRotOffset.y = func_0021170C(p->m_vHeadWobbleRotOffset.y, 0.087266475f);

    speed = p->m_Speed;
    if (speed < 0.0f) {
        amount = -speed * 0.032552086f;
    } else {
        amount = speed * 0.032552086f;
    }
    if (amount < 0.0f) {
        amount = 0.0f;
    } else if (amount > 1.0f) {
        amount = 1.0f;
    }

    phase = -(p->m_ModeTime * p->m_CameraMovementScaler * 360.0f) * 0.0014544412f;
    if (p->m_CharacterActor.header.m_pIntelligence != NULL) {
        base = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_CollisionHeight * 0.9f;
    } else {
        base = 82.94399f;
    }
    if (func_002982F0(phase) * 15.36f < 0.0f) {
        y = base - amount * -(func_002982F0(phase) * 15.36f);
    } else {
        y = base - amount * 15.36f * func_002982F0(phase);
    }
    func_002117DC(&p->m_vEyeOffset.y, y, 0.5f);
    side = amount * (func_002974C0(phase) * 7.68f);
    func_002117DC(&p->m_vEyeOffset.x, -func_002982F0(obj->yaw - 1.5707964f) * side, 0.5f);
    func_002117DC(&p->m_vEyeOffset.z, -func_002974C0(obj->yaw - 1.5707964f) * side, 0.5f);
    func_002117DC(&p->m_vHeadWobbleRotOffset.z, -(side * 0.0017044232f), 0.25f);
}
