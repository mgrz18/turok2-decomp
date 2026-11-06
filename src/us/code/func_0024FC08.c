#include "common.h"
#include "structs/intelligence.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ f32 yaw;
} BobObject;

void Math_EaseToward(f32 *value, f32 target, f32 rate);
f32 Math_DecayToZero(f32 value, f32 rate);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);

/* Settles the view tilt and kicks, then bobs the view with the walk. */
void CPlayer__BobView(CPlayer *p, BobObject *obj) {
    f32 phase;
    f32 base;
    f32 amount;

    Math_EaseToward(&p->m_vHeadRotOffset.z, 0.0f, 0.25f);
    Math_EaseToward(&p->m_vHeadRotOffset.y, 0.0f, 0.5f);
    p->m_vHeadWobbleRotOffset.x = Math_DecayToZero(p->m_vHeadWobbleRotOffset.x, 0.087266475f);
    p->m_vHeadWobbleRotOffset.y = Math_DecayToZero(p->m_vHeadWobbleRotOffset.y, 0.087266475f);
    p->m_vHeadWobbleRotOffset.z = Math_DecayToZero(p->m_vHeadWobbleRotOffset.z, 0.087266475f);

    if (p->m_BreathSpeed > 21.0f) {
        p->m_BreathSpeed = 21.0f;
    }
    if (p->m_BreathAmp > 0.1f) {
        p->m_BreathAmp = 0.1f;
    }
    p->m_BreathSpeed = Math_DecayToZero(p->m_BreathSpeed, 0.11666667f);
    p->m_BreathAmp = Math_DecayToZero(p->m_BreathAmp, 0.00055555557f);
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
    Math_EaseToward(&p->m_vEyeOffset.y, base + p->m_BreathAmp * (func_002982F0(phase) * 10.24f), 0.5f);
    amount = p->m_BreathAmp * (func_002974C0(phase) * 8.192f);
    Math_EaseToward(&p->m_vEyeOffset.x, -func_002982F0(obj->yaw + 3.1415927f) * amount, 0.5f);
    Math_EaseToward(&p->m_vEyeOffset.z, -func_002974C0(obj->yaw + 3.1415927f) * amount, 0.5f);
}
