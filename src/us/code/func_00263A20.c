/* Boundary fixed in versions/cuts.us.txt: splat's func_00263A14 starts with 3 orphan words (addu a0,s0; addu a1,s0;
 * jal func_0020EEF8) whose delay slot is this function's first instruction. The real
 * function is func_00263A20 (called from func_00264378); this file matches it from
 * 0x263A20 to the end of splat's func_00263A14. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 flags;
    /* 0x28 */ u8 pad28[0x54 - 0x28];
    /* 0x54 */ f32 triggerDelay;
    /* 0x58 */ f32 speed;
    /* 0x5C */ f32 duration;
} Params;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Params params;
} Def;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Def *def;
    /* 0x18 */ u8 pad18[0xD4 - 0x18];
    /* 0xD4 */ s32 flags;
} Self;

typedef struct {
    /* 0x000 */ s32 flags;
    /* 0x004 */ u8 pad4[0x35 - 0x4];
    /* 0x035 */ s8 unk35;
    /* 0x036 */ u8 pad36;
    /* 0x037 */ s8 state;
    /* 0x038 */ u8 pad38[0x44 - 0x38];
    /* 0x044 */ f32 timer;
    /* 0x048 */ u8 pad48[0x10C - 0x48];
    /* 0x10C */ f32 triggerTime;
    /* 0x110 */ f32 shake[3];
} AI;

extern f32 D_800B6D28; /* frame time */

f32 func_002119FC(f32 lo, f32 hi);
f32 func_00266F50(f32 x);

static __inline__ s32 canClose(Params *params, AI *ai) {
    if ((params->flags & 0x8000000) && !(ai->flags & 0x20000)) {
        return 0;
    }
    if ((params->flags & 0x10000000) && (ai->flags & 0x20000)) {
        return 0;
    }
    return 1;
}

void func_00263A20(Self *self, AI *ai) {
    Params *params;
    s32 oldState;
    s32 flags;

    params = &self->def->params;
    oldState = ai->state;
    switch (oldState) {
        case 0:
            flags = params->flags;
            if (!(flags & 0xC00000)) {
                break;
            }
            if (flags & 0x4000000) {
                if (ai->flags & 0x20000) {
                    ai->triggerTime += D_800B6D28;
                } else {
                    ai->triggerTime = 0.0f;
                    break;
                }
            } else if (ai->triggerTime > 0.0f || (ai->flags & 0x10000)) {
                ai->triggerTime += D_800B6D28;
            }
            if (ai->triggerTime >= params->triggerDelay) {
                ai->timer = 0.0f;
                flags = params->flags;
                if (flags & 0x1000000) {
                    ai->state = 3;
                } else if (flags & 0x400000) {
                    ai->state = 1;
                } else if (flags & 0x800000) {
                    ai->state = 4;
                }
            }
            break;
        case 3:
            ai->shake[0] = func_002119FC(-0.512f, 0.512f);
            ai->shake[1] = func_002119FC(-0.512f, 0.512f);
            ai->shake[2] = func_002119FC(-0.512f, 0.512f);
            ai->timer += D_800B6D28;
            if (ai->timer > 45.0f) {
                ai->timer = 0.0f;
                flags = params->flags;
                if (flags & 0x400000) {
                    ai->state = 1;
                } else if (flags & 0x800000) {
                    ai->state = 4;
                }
            }
            break;
        case 1:
            if (ai->timer < params->duration) {
                ai->timer += D_800B6D28;
            } else if (params->flags & 0x2000000) {
                self->flags &= ~0x2000;
            } else {
                ai->timer = params->duration;
                if (canClose(params, ai)) {
                    ai->state = 2;
                }
            }
            if (params->duration > 0.0f) {
                ai->shake[1] = -params->speed * func_00266F50(ai->timer / params->duration);
            } else {
                ai->shake[1] = -params->speed;
            }
            break;
        case 2:
            if (ai->timer > 0.0f) {
                ai->timer -= D_800B6D28;
            } else {
                ai->state = 0;
                ai->triggerTime = 0.0f;
            }
            if (params->duration > 0.0f) {
                ai->shake[1] = -params->speed * func_00266F50(ai->timer / params->duration);
            } else {
                ai->shake[1] = -params->speed;
            }
            break;
        case 4:
            if (ai->timer < params->duration) {
                ai->timer += D_800B6D28;
            }
            if (params->duration > 0.0f) {
                ai->shake[1] = -params->speed * func_00266F50(ai->timer / params->duration);
            } else {
                ai->shake[1] = -params->speed;
            }
            if (params->flags & 0x2000000) {
                self->flags &= ~0x2000;
            }
            break;
    }
    if (oldState != ai->state) {
        ai->unk35 = -1;
    }
}
