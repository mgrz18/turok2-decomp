#include "common.h"

/* Flying AI steering, scaled by speed: like func_00260730, but the bank and
 * pitch ease faster when they have to swing to the other side. */

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 velY;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ f32 bank;
    /* 0x110 */ f32 pitch;
} AI;

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

s32 func_0025E428(Actor *self, void *ai);
f32 func_0021160C(f32 value, f32 target, f32 step);
f32 func_00211774(f32 cur, f32 target, f32 rate);
void func_00245BAC(Actor *self, AI *ai, f32 angle, f32 turn);

void func_00260D60(Actor *self, AI *ai, Steer *steer, Limits *limits, f32 speed, f32 turnRate) {
    f32 v;
    f32 max;
    f32 bank;
    f32 pitch;
    f32 bankSide;
    f32 pitchSide;

    func_00245BAC(self, ai, turnRate * 0.13962635f, steer->turn);
    if (func_0025E428(self, ai) == 1) {
        v = func_0021160C(self->velY, steer->wantVelY, limits->accel * (speed * 8.0f));
        self->velY = v;
        max = speed * 102.399994f;
        if (v > max) {
            self->velY = max;
        } else if (v < -max) {
            self->velY = -max;
        }
        bank = self->velY * 0.008726647f;
        if (bank > 0.436332345f) {
            bank = 0.436332345f;
        } else if (bank < -0.436332345f) {
            bank = -0.436332345f;
        }
        bankSide = 1.0f;
        if (bank < 0.0f) {
            bankSide = -1.0f;
        }
        if (bankSide != (ai->bank < 0.0f ? -1.0f : 1.0f)) {
            ai->bank = func_00211774(ai->bank, bank, 0.17453295f);
        } else {
            ai->bank = func_00211774(ai->bank, bank, 0.087266475f);
        }
        pitch = steer->turn * 0.75f;
        if (pitch > 0.436332345f) {
            pitch = 0.436332345f;
        } else if (pitch < -0.436332345f) {
            pitch = -0.436332345f;
        }
        pitchSide = 1.0f;
        if (pitch < 0.0f) {
            pitchSide = -1.0f;
        }
        if (pitchSide != (ai->pitch < 0.0f ? -1.0f : 1.0f)) {
            ai->pitch = func_00211774(ai->pitch, pitch, 0.3490659f);
        } else {
            ai->pitch = func_00211774(ai->pitch, pitch, 0.17453295f);
        }
    }
}
