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

/* Settles the view tilt and kicks, then bobs the view with the walk. */
void func_0024FC08(CPlayer *p, BobObject *obj) {
    f32 phase;
    f32 base;
    f32 amount;

    func_002117DC(&p->m_vHeadRotOffset.z, 0.0f, 0.25f);
    func_002117DC(&p->m_vHeadRotOffset.y, 0.0f, 0.5f);
    p->m_vHeadWobbleRotOffset.x = func_0021170C(p->m_vHeadWobbleRotOffset.x, 0.087266475f);
    p->m_vHeadWobbleRotOffset.y = func_0021170C(p->m_vHeadWobbleRotOffset.y, 0.087266475f);
    p->m_vHeadWobbleRotOffset.z = func_0021170C(p->m_vHeadWobbleRotOffset.z, 0.087266475f);

    if (p->m_BreathSpeed > 21.0f) {
        p->m_BreathSpeed = 21.0f;
    }
    if (p->m_BreathAmp > 0.1f) {
        p->m_BreathAmp = 0.1f;
    }
    p->m_BreathSpeed = func_0021170C(p->m_BreathSpeed, 0.11666667f);
    p->m_BreathAmp = func_0021170C(p->m_BreathAmp, 0.00055555557f);
    if (p->m_BreathAmp < 0.06f) {
        p->m_BreathAmp = 0.0f;
        p->m_BreathSpeed = 0.0f;
    }

    phase = p->m_ModeTime * p->m_BreathSpeed * 0.017453294f;
    if (p->m_CharacterActor.header.m_pIntelligence != NULL) {
        base = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_CollisionHeight * 0.9f;
    } else {
        base = 82.94399f;
    }
    func_002117DC(&p->m_vEyeOffset.y, base + p->m_BreathAmp * (func_002982F0(phase) * 10.24f), 0.5f);
    amount = p->m_BreathAmp * (func_002974C0(phase) * 8.192f);
    func_002117DC(&p->m_vEyeOffset.x, -func_002982F0(obj->yaw + 3.1415927f) * amount, 0.5f);
    func_002117DC(&p->m_vEyeOffset.z, -func_002974C0(obj->yaw + 3.1415927f) * amount, 0.5f);
}
