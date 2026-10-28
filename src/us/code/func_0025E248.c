#include "common.h"

/* Index-2 attack follow-up: in the 7020-7041 animation band it chains to the
 * next move; otherwise it finishes the current one. */

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
} AttackInfo;

typedef struct {
    /* 0x00 */ u8 pad00[0x14];
    /* 0x14 */ u8 *def;
    /* 0x18 */ u8 pad18[0xBC];
    /* 0xD4 */ s32 flags;
} AttackActor;

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad04[0x30];
    /* 0x34 */ s8 move;
    /* 0x35 */ u8 pad35[0xB];
    /* 0x40 */ f32 time;
    /* 0x44 */ u8 pad44[0x80];
    /* 0xC4 */ s16 anim;
    /* 0xC6 */ s8 animSlot;
    /* 0xC7 */ s8 queued;
    /* 0xC8 */ u8 padC8[0x6B];
    /* 0x133 */ s8 index;
} AttackState;

s32 func_00219F74(void *, s32, s32);
s32 func_0025AA78(void *, void *);
s32 func_0025AAA4(void *, void *);
s32 func_002685F0(void *);
void func_0024646C(void *, void *, s32, s32, s32);
void func_00243414(void *, void *, s32);
void func_00246918(void *, void *);
void func_002280B0(void *);

void func_0025E248(AttackActor *actor, AttackState *state) {
    AttackInfo *info = (AttackInfo *)(actor->def + 0x14);
    s32 anim;

    if (state->index == 2) {
        switch (state->anim) {
        case 7020:
        case 7025:
        case 7030:
        case 7035:
        case 7040:
            if (state->queued == 0) {
                return;
            }
            anim = func_0025AA78(actor, state);
            break;
        case 7021:
        case 7026:
        case 7031:
        case 7036:
        case 7041:
            if (!func_002685F0(actor)) {
                return;
            }
            anim = func_0025AAA4(actor, state);
            break;
        default:
            goto finish;
        }
        state->animSlot = func_00219F74(actor, anim, -1);
        state->anim = anim;
        state->queued = 0;
        state->flags &= ~1;
        return;
    }
finish:
    if (state->queued != 0) {
        if (state->move == 0x35) {
            func_00243414(actor, state, 0x36);
        } else {
            state->flags |= 0x20;
            actor->flags &= ~0x01000000;
            func_0024646C(actor, state, 4, info->unk0C, info->unk10);
            actor->flags &= ~0x20000;
            if (state->time > 90.0f) {
                func_00243414(actor, state, 0x3C);
            }
        }
        if (info->flags & 0x10000) {
            actor->flags &= ~0x100;
            actor->flags &= ~0x2000;
            func_00246918(actor, state);
            if (actor->flags & 0x80000) {
                func_002280B0(actor);
            }
        }
    } else {
        state->time = 0.0f;
    }
}
