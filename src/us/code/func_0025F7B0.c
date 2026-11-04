#include "common.h"
#include "structs/game_object.h"

/* AI state: bob towards the target height on a sine wave, the vertical speed
 * eased and clamped; once level with it, switch to state 0x3C. */

extern f32 D_800B6D28;
void func_0025E9BC(CGameObjectInstance *self, CAI *ai);
void func_00246108(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
f32 func_002982F0(f32 x); /* sinf */
f32 func_00211774(f32 cur, f32 target, f32 rate);
void func_00243414(CGameObjectInstance *self, CAI *ai, s32 mode);

void func_0025F7B0(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    f32 v;

    if (ai->m_Mode == 0x3C) {
        func_0025E9BC(self, ai);
    }
    func_00246108(self, ai, &t, 1, 0);
    ai->m_ConditionTimer += D_800B6D28 * 15.0f;
    t.m_Path.m_vDelta.y += func_002982F0(ai->m_ConditionTimer * 0.017453294f) * 61.44f + 51.199997f;
    v = func_00211774(self->m_vVelocity.y, t.m_Path.m_vDelta.y, 10.24f);
    self->m_vVelocity.y = v;
    if (v > 40.96f) {
        self->m_vVelocity.y = 40.96f;
    } else if (v < -40.96f) {
        self->m_vVelocity.y = -40.96f;
    }
    if (t.m_Path.m_vPos.y <= self->header.m_vPos.y + self->m_vVelocity.y) {
        self->m_vVelocity.y = t.m_Path.m_vDelta.y;
        if (ai->m_Mode != 0x3C) {
            func_00243414(self, ai, 0x3C);
        }
    }
}
