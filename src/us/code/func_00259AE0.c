#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x934];
    /* 0x934 */ s16 unk934[1];
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0xD8];
    /* 0x0D8 */ f32 unkD8;
    /* 0x0DC */ u8 padDC[0x1A8 - 0xDC];
    /* 0x1A8 */ Player *owner;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 unk34;
    /* 0x035 */ u8 pad35[0xC4 - 0x35];
    /* 0x0C4 */ s16 unkC4;
    /* 0x0C6 */ u8 padC6[0x10C - 0xC6];
    /* 0x10C */ f32 unk10C;
    /* 0x110 */ f32 unk110;
    /* 0x114 */ u8 pad114[0x128 - 0x114];
    /* 0x128 */ s32 unk128;
    /* 0x12C */ s32 unk12C;
} State;

/* Frame globals; the time step (D_800B6D28) must be read through the block to match. */
extern struct {
    /* 0x0 */ u32 frame;
    /* 0x4 */ u32 pad4;
    /* 0x8 */ f32 delta;
} D_800B6D20;
f32 func_00211774(f32 value, f32 target, f32 rate);
s16 *func_0025398C(Player *player, s32 id);

void func_00259AE0(Actor *actor, State *state) {
    f32 v;

    v = func_00211774(state->unk110, 0.174532949924469f /* 10 deg */, 0.0175f);
    state->unk110 = v;
    state->unk10C += v * D_800B6D20.delta;
    if (state->unk34 == 4 && state->unkC4 != 0x4E84) {
        if (7.0f <= actor->unkD8) {
            if (state->unk128 == 0) {
                state->unk12C = 0;
            }
            state->unk128 = 1;
        } else {
            state->unk128 = 0;
        }
    } else {
        Player *player = actor->owner;

        if (*(s16 *) ((s8 *) player + *func_0025398C(player, 0x15) * 2 + 0x934) != 0) {
            state->unk128 = 0;
        } else {
            state->unk128 = 1;
        }
    }
}
