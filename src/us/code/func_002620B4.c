#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x4C];
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 pad50[0x60 - 0x50];
    /* 0x60 */ f32 unk60;
} Arg2;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 unk18;
} Arg3;

f32 Math_StepClamped(f32 value, f32 delta, f32 limit);
void AI_TurnClamped(CGameObjectInstance *self, CAI *ai, f32 angle, f32 arg3);

void EnemySwim_SteerToPath(CGameObjectInstance *self, CAI *ai, Arg2 *a2, Arg3 *a3) {
    self->m_vVelocity.y = Math_StepClamped(self->m_vVelocity.y, a2->unk60, a3->unk18 * 2.0f);
    if (self->m_vVelocity.y > 81.92f) {
        self->m_vVelocity.y = 81.92f;
    } else if (self->m_vVelocity.y < -81.92f) {
        self->m_vVelocity.y = -81.92f;
    }
    AI_TurnClamped(self, ai, ai->m_Enemy.m_TurnRate, a2->unk4C);
}
