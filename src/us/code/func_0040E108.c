#include "common.h"

typedef struct {
    /* 0x00 */ u32 kind;
    /* 0x04 */ s32 visible;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 padC[0x40 - 0xC];
    /* 0x40 */ f32 unk40;
} Target;

typedef struct {
    /* 0x00 */ Target ground;
    /* 0x44 */ Target air;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ f32 yaw;
    /* 0x54 */ u8 pad54[0xB8 - 0x54];
    /* 0xB8 */ u16 type;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x24];
    /* 0x024 */ s32 unk24;
    /* 0x028 */ u8 pad28[0x98 - 0x28];
    /* 0x098 */ f32 baseYaw;
    /* 0x09C */ u8 pad9C[0x11C - 0x9C];
    /* 0x11C */ f32 unk11C;
} CAI;

extern void func_00246108(Actor *actor, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_0025AD90(Actor *actor, CAI *ai, f32 angle);
extern void func_00210EF0(f32 *angle); /* wraps an angle */
extern void func_00245BAC(Actor *actor, CAI *ai, f32 turnRate, f32 angle);

void func_0040E108(Actor *actor, CAI *ai) {
    Targets t;
    f32 delta;
    f32 rate;
    f32 range;
    f32 turn;
    f32 half;
    f32 sum;

    func_00246108(actor, ai, &t, 0, 1);
    ai->unk11C = t.ground.unk40;
    switch (actor->type) {
        case 0x454:
            rate = 0.06981318f;
            range = 6.283186f;
            func_0025AD90(actor, ai, -1.5707965f);
            ai->unk24 = 0;
            break;
        case 0x455:
            rate = 0.06981318f;
            range = 0.78539824f;
            break;
        case 0x456:
            rate = 0.06981318f;
            range = 0.52359885f;
            break;
        default:
            range = rate = 0.0f;
            break;
    }
    delta = actor->yaw - ai->baseYaw;
    func_00210EF0(&delta);
    turn = t.ground.angle;
    if (range < 6.283186f) {
        half = range * 0.5f;
        sum = delta + turn;
        if (sum > half) {
            turn = half - delta;
        } else if (sum < -half) {
            turn = -half - delta;
        }
    }
    func_00245BAC(actor, ai, rate, turn);
}
