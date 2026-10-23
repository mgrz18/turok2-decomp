#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 anim;
} AnimOverride;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ AnimOverride override;
} ActorDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorDef *def;
    /* 0x18 */ u8 pad18[0xB8 - 0x18];
    /* 0xB8 */ u16 type;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x130];
    /* 0x130 */ s8 unk130;
    /* 0x131 */ s8 unk131;
} State;

s32 func_00259E60(Actor *actor, State *state);
s32 func_00259E8C(Actor *actor, State *state);
s32 func_00259EB8(Actor *actor, State *state);

s32 func_0025E6E0(Actor *actor, State *state) {
    AnimOverride *ov = &actor->def->override;

    if (ov->anim != 0) {
        if (actor->type == 0x1F9 || actor->type == 0x385) {
            switch (ov->anim) {
                case 0x2B16:
                case 0x2B20:
                case 0x2B2A:
                case 0x2B34:
                case 0x2B3E:
                    return ov->anim + 1;
                default:
                    return ov->anim;
            }
        }
        return ov->anim;
    }
    if (state->unk130) {
        return func_00259E60(actor, state);
    }
    if (state->unk131) {
        return func_00259E8C(actor, state);
    }
    return func_00259EB8(actor, state);
}
