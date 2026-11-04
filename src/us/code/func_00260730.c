#include "common.h"
#include "structs/game_object.h"

/* Flying CAI steering: for index 1, ease the vertical speed towards the wanted
 * one and bank/pitch with it; then turn. Against a target of another kind,
 * re-roll the attack delay. */

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 mode;
    /* 0x48 */ CGameObjectInstance *target;
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
f32 func_002119FC(f32 lo, f32 hi);

void func_00260730(CGameObjectInstance *self, CAI *ai, Steer *steer, Limits *limits) {
    CGameObjectInstance *target = steer->target;
    f32 v;
    f32 bank;
    f32 pitch;

    if (func_0025E428(self, ai) == 1) {
        v = func_0021160C(self->m_vVelocity.y, steer->wantVelY, limits->accel * 8.0f);
        self->m_vVelocity.y = v;
        if (v > 81.92f) {
            self->m_vVelocity.y = 81.92f;
        } else if (v < -81.92f) {
            self->m_vVelocity.y = -81.92f;
        }
        bank = -self->m_vVelocity.y * 0.030543264f;
        if (bank > 0.436332345f) {
            bank = 0.436332345f;
        } else if (bank < -0.436332345f) {
            bank = -0.436332345f;
        }
        ai->m_VarF32[0] = func_00211774(ai->m_VarF32[0], bank, 0.087266475f);
        pitch = steer->turn * 0.75f;
        if (pitch > 0.436332345f) {
            pitch = 0.436332345f;
        } else if (pitch < -0.436332345f) {
            pitch = -0.436332345f;
        }
        ai->m_VarF32[1] = func_00211774(ai->m_VarF32[1], pitch, 0.17453295f);
    }
    func_00245BAC(self, ai, 0.13962635f, steer->turn);
    if (steer->mode == 2 && target != NULL && ai->m_VarF32[8] > 15.0f) {
        if (func_0025E428(self, ai) != func_0025E428(target, (u8 *)target + 0x140)) {
            ai->m_VarF32[8] = func_002119FC(0.0f, 15.0f);
        }
    }
}
