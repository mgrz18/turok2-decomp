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
    /* 0xA14 */ u8 padA14[0xACC - 0xA14];
    /* 0xACC */ f32 roll;
    /* 0xAD0 */ f32 tilt;
    /* 0xAD4 */ f32 kickX;
    /* 0xAD8 */ f32 kickY;
    /* 0xADC */ f32 kickZ;
    /* 0xAE0 */ f32 bobX;
    /* 0xAE4 */ f32 bobY;
    /* 0xAE8 */ f32 bobZ;
    /* 0xAEC */ u8 padAEC[0xAFC - 0xAEC];
    /* 0xAFC */ f32 bobAmount;
    /* 0xB00 */ f32 bobSpeed;
} BobPlayer;

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ f32 yaw;
} BobObject;

void func_002117DC(f32 *value, f32 target, f32 rate);
f32 func_0021170C(f32 value, f32 rate);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);

/* Settles the view tilt and kicks, then bobs the view with the walk. */
void func_0024FC08(BobPlayer *p, BobObject *obj) {
    f32 phase;
    f32 base;
    f32 amount;

    func_002117DC(&p->tilt, 0.0f, 0.25f);
    func_002117DC(&p->roll, 0.0f, 0.5f);
    p->kickX = func_0021170C(p->kickX, 0.087266475f);
    p->kickY = func_0021170C(p->kickY, 0.087266475f);
    p->kickZ = func_0021170C(p->kickZ, 0.087266475f);

    if (p->bobSpeed > 21.0f) {
        p->bobSpeed = 21.0f;
    }
    if (p->bobAmount > 0.1f) {
        p->bobAmount = 0.1f;
    }
    p->bobSpeed = func_0021170C(p->bobSpeed, 0.11666667f);
    p->bobAmount = func_0021170C(p->bobAmount, 0.00055555557f);
    if (p->bobAmount < 0.06f) {
        p->bobAmount = 0.0f;
        p->bobSpeed = 0.0f;
    }

    phase = p->time * p->bobSpeed * 0.017453294f;
    if (p->def != NULL) {
        base = p->def->eyeHeight * 0.9f;
    } else {
        base = 82.94399f;
    }
    func_002117DC(&p->bobY, base + p->bobAmount * (func_002982F0(phase) * 10.24f), 0.5f);
    amount = p->bobAmount * (func_002974C0(phase) * 8.192f);
    func_002117DC(&p->bobX, -func_002982F0(obj->yaw + 3.1415927f) * amount, 0.5f);
    func_002117DC(&p->bobZ, -func_002974C0(obj->yaw + 3.1415927f) * amount, 0.5f);
}
