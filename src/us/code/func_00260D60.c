#include "common.h"
#include "structs/game_object.h"

/* Flying AI steering, scaled by speed: like func_00260730, but the pitch and
 * roll ease faster when they have to swing to the other side. */

typedef struct {
    /* 0x00 */ u8 pad0[0x4C];
    /* 0x4C */ f32 turn;
    /* 0x50 */ u8 pad50[0x10];
    /* 0x60 */ f32 wantVelY;
} Steer;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 accel;
} Limits;

s32 func_0025E428(CGameObjectInstance *self, void *ai);
f32 func_0021160C(f32 value, f32 target, f32 step);
f32 func_00211774(f32 cur, f32 target, f32 rate);
void func_00245BAC(CGameObjectInstance *self, CAI *ai, f32 angle, f32 turn);

void func_00260D60(CGameObjectInstance *self, CAI *ai, Steer *steer, Limits *limits, f32 speed, f32 turnRate) {
    f32 v;
    f32 max;
    f32 pitch;
    f32 roll;
    f32 pitchSide;
    f32 rollSide;

    func_00245BAC(self, ai, turnRate * 0.13962635f, steer->turn);
    if (func_0025E428(self, ai) == 1) {
        v = func_0021160C(self->m_vVelocity.y, steer->wantVelY, limits->accel * (speed * 8.0f));
        self->m_vVelocity.y = v;
        max = speed * 102.399994f;
        if (v > max) {
            self->m_vVelocity.y = max;
        } else if (v < -max) {
            self->m_vVelocity.y = -max;
        }
        pitch = self->m_vVelocity.y * 0.008726647f;
        if (pitch > 0.436332345f) {
            pitch = 0.436332345f;
        } else if (pitch < -0.436332345f) {
            pitch = -0.436332345f;
        }
        pitchSide = 1.0f;
        if (pitch < 0.0f) {
            pitchSide = -1.0f;
        }
        if (pitchSide != (ai->m_Enemy.m_Pitch < 0.0f ? -1.0f : 1.0f)) {
            ai->m_Enemy.m_Pitch = func_00211774(ai->m_Enemy.m_Pitch, pitch, 0.17453295f);
        } else {
            ai->m_Enemy.m_Pitch = func_00211774(ai->m_Enemy.m_Pitch, pitch, 0.087266475f);
        }
        roll = steer->turn * 0.75f;
        if (roll > 0.436332345f) {
            roll = 0.436332345f;
        } else if (roll < -0.436332345f) {
            roll = -0.436332345f;
        }
        rollSide = 1.0f;
        if (roll < 0.0f) {
            rollSide = -1.0f;
        }
        if (rollSide != (ai->m_Enemy.m_Roll < 0.0f ? -1.0f : 1.0f)) {
            ai->m_Enemy.m_Roll = func_00211774(ai->m_Enemy.m_Roll, roll, 0.3490659f);
        } else {
            ai->m_Enemy.m_Roll = func_00211774(ai->m_Enemy.m_Roll, roll, 0.17453295f);
        }
    }
}
