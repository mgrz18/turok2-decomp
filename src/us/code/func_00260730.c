#include "common.h"

/* Flying AI steering: for index 1, ease the vertical speed towards the wanted
 * one and bank/pitch with it; then turn. Against a target of another kind,
 * re-roll the attack delay. */

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 velY;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ f32 bank;
    /* 0x110 */ f32 pitch;
    /* 0x114 */ u8 pad114[0x18];
    /* 0x12C */ f32 attackDelay;
} AI;

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 mode;
    /* 0x48 */ Actor *target;
    /* 0x4C */ f32 turn;
    /* 0x50 */ u8 pad50[0x10];
    /* 0x60 */ f32 wantVelY;
} Steer;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 accel;
} Limits;

s32 func_0025E428(Actor *self, void *ai);
f32 func_0021160C(f32 value, f32 target, f32 step);
f32 func_00211774(f32 cur, f32 target, f32 rate);
void func_00245BAC(Actor *self, AI *ai, f32 angle, f32 turn);
f32 func_002119FC(f32 lo, f32 hi);

void func_00260730(Actor *self, AI *ai, Steer *steer, Limits *limits) {
    Actor *target = steer->target;
    f32 v;
    f32 bank;
    f32 pitch;

    if (func_0025E428(self, ai) == 1) {
        v = func_0021160C(self->velY, steer->wantVelY, limits->accel * 8.0f);
        self->velY = v;
        if (v > 81.92f) {
            self->velY = 81.92f;
        } else if (v < -81.92f) {
            self->velY = -81.92f;
        }
        bank = -self->velY * 0.030543264f;
        if (bank > 0.436332345f) {
            bank = 0.436332345f;
        } else if (bank < -0.436332345f) {
            bank = -0.436332345f;
        }
        ai->bank = func_00211774(ai->bank, bank, 0.087266475f);
        pitch = steer->turn * 0.75f;
        if (pitch > 0.436332345f) {
            pitch = 0.436332345f;
        } else if (pitch < -0.436332345f) {
            pitch = -0.436332345f;
        }
        ai->pitch = func_00211774(ai->pitch, pitch, 0.17453295f);
    }
    func_00245BAC(self, ai, 0.13962635f, steer->turn);
    if (steer->mode == 2 && target != NULL && ai->attackDelay > 15.0f) {
        if (func_0025E428(self, ai) != func_0025E428(target, (u8 *)target + 0x140)) {
            ai->attackDelay = func_002119FC(0.0f, 15.0f);
        }
    }
}
