#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ f32 eyeHeight;
} BobDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ BobDef *def;
    /* 0x018 */ u8 pad18[0xA10 - 0x18];
    /* 0xA10 */ f32 time;
    /* 0xA14 */ u8 padA14[0xA78 - 0xA14];
    /* 0xA78 */ f32 speed;
    /* 0xA7C */ u8 padA7C[0xACC - 0xA7C];
    /* 0xACC */ f32 roll;
    /* 0xAD0 */ f32 tilt;
    /* 0xAD4 */ f32 kickX;
    /* 0xAD8 */ f32 kickY;
    /* 0xADC */ f32 kickZ;
    /* 0xAE0 */ f32 bobX;
    /* 0xAE4 */ f32 bobY;
    /* 0xAE8 */ f32 bobZ;
    /* 0xAEC */ u8 padAEC[0xB28 - 0xAEC];
    /* 0xB28 */ f32 strideRate;
} BobPlayer;

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
void func_0024FE10(BobPlayer *p, BobObject *obj) {
    f32 speed;
    f32 amount;
    f32 phase;
    f32 base;
    f32 y;
    f32 side;

    func_002117DC(&p->tilt, 0.0f, 0.25f);
    func_002117DC(&p->roll, 0.0f, 0.5f);
    p->kickX = func_0021170C(p->kickX, 0.087266475f);
    p->kickY = func_0021170C(p->kickY, 0.087266475f);

    speed = p->speed;
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

    phase = -(p->time * p->strideRate * 360.0f) * 0.0014544412f;
    if (p->def != NULL) {
        base = p->def->eyeHeight * 0.9f;
    } else {
        base = 82.94399f;
    }
    if (func_002982F0(phase) * 15.36f < 0.0f) {
        y = base - amount * -(func_002982F0(phase) * 15.36f);
    } else {
        y = base - amount * 15.36f * func_002982F0(phase);
    }
    func_002117DC(&p->bobY, y, 0.5f);
    side = amount * (func_002974C0(phase) * 7.68f);
    func_002117DC(&p->bobX, -func_002982F0(obj->yaw - 1.5707964f) * side, 0.5f);
    func_002117DC(&p->bobZ, -func_002974C0(obj->yaw - 1.5707964f) * side, 0.5f);
    func_002117DC(&p->kickZ, -(side * 0.0017044232f), 0.25f);
}
