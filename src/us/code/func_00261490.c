#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ u8 pad20[0x34 - 0x20];
    /* 0x34 */ f32 scale[3];
} Self;

extern f32 D_800B6D28; /* frame time */

f32 func_00211774(f32 cur, f32 target, f32 rate);
f32 func_002974C0(f32 x);
f32 func_002119FC(f32 lo, f32 hi);

void func_00261490(Self *self, CAI *ai) {
    f32 pitch;
    f32 base;
    f32 c;

    pitch = -self->unk1C * 0.030543264f;
    if (pitch > 1.308997f) {
        pitch = 1.308997f;
    } else if (pitch < -1.308997f) {
        pitch = -1.308997f;
    }
    ai->m_Enemy.m_Pitch = func_00211774(ai->m_Enemy.m_Pitch, pitch, 0.06981318f);

    if (ai->m_Health != 0) {
        if (ai->m_Mode == 27) {
            c = func_002974C0(ai->m_ModeTime * 0.31415930f);
            base = ai->m_Enemy.m_BaseScale;
            base += (c + 1.0f) * ((base * 1.25f - base) * 0.5f);
            self->scale[0] = func_00211774(self->scale[0], base * 3.0f * 2.0f, 0.1f);
            self->scale[1] = func_00211774(self->scale[1], base * 1.25f * 2.0f, 0.1f);
            self->scale[2] = func_00211774(self->scale[2], base * 1.25f * 2.0f, 0.1f);
        } else {
            self->scale[0] = func_00211774(self->scale[0], ai->m_Enemy.m_BaseScale, 0.1f);
            self->scale[1] = func_00211774(self->scale[1], ai->m_Enemy.m_BaseScale, 0.1f);
            self->scale[2] = func_00211774(self->scale[2], ai->m_Enemy.m_BaseScale, 0.1f);
        }
    }

    ai->m_Enemy.m_WanderTimer -= D_800B6D28;
    if (ai->m_Enemy.m_WanderTimer <= 0.0f) {
        ai->m_Enemy.m_WanderTimer = func_002119FC(60.0f, 120.0f);
        ai->m_vTargetOffset.x = func_002119FC(-51.199997f, 51.199997f);
        ai->m_vTargetOffset.y = func_002119FC(-51.199997f, 51.199997f);
        ai->m_vTargetOffset.z = func_002119FC(-51.199997f, 51.199997f);
    }
}
