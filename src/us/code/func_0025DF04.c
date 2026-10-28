#include "common.h"

/* Picks the attack for an AI actor: a special one against a flagged target,
 * then the ranged/melee/close sets allowed by the mode bits (20-25), the
 * index (0..2) choosing which 5-bit group of the def mask enables it. */

typedef struct {
    /* 0x00 */ u8 pad00[0x10];
    /* 0x10 */ s32 target;
    /* 0x14 */ s32 *def;
} AttackActor;

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad04[0xC0];
    /* 0xC4 */ s16 anim;
    /* 0xC6 */ u8 padC6[0x6D];
    /* 0x133 */ s8 index;
} AttackState;

typedef struct {
    /* 0x00 */ u8 pad00[0x44];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0xA];
    /* 0x52 */ u16 flags2;
} TargetInfo;

extern u8 D_800F7078[];
TargetInfo *func_0022425C(void *, s32);
s32 func_00219F74(void *, s32, s32);
s32 func_0025A83C(void *, void *);
s32 func_0025A868(void *, void *);
s32 func_0025A894(void *, void *);
s32 func_0025A8C0(void *, void *);
s32 func_0025A8EC(void *, void *);
s32 func_0025A918(void *, void *);
s32 func_0025A944(void *, void *);
s32 func_0025A970(void *, void *);
s32 func_0025A99C(void *, void *);
s32 func_0025A9C8(void *, void *);
s32 func_0025A9F4(void *, void *);
s32 func_0025AA20(void *, void *);
s32 func_0025AA4C(void *, void *);

s32 func_0025DF04(AttackActor *actor, AttackState *state) {
    s32 result = -1;
    s32 flags = state->flags;
    s32 *def = actor->def;
    s32 index = state->index;
    s32 mode = flags & 0x03F00000;
    s32 target = actor->target;
    u16 *masks = (u16 *)((u8 *)def + 0x14); /* [0x3C] close, [0x3D] mid, [0x3E] far */
    s32 rest = flags & ~0x03F00000;
    TargetInfo *t;

    if (*def != 1) {
        return 0;
    }
    if (target != 0) {
        t = func_0022425C(D_800F7078, target);
        if (t != NULL && ((t->flags & 0x400000) || (t->flags2 & 0xC0))) {
            result = func_0025AA4C(actor, state);
            if (result != -1) {
                return result;
            }
        }
    }
    if (mode & 0xC00000) {
        if (index == 0 && (masks[0x3E] & 0x1F)) {
            result = func_0025A9C8(actor, state);
        } else if (index == 1 && (masks[0x3E] & 0x3E0)) {
            result = func_0025A9F4(actor, state);
        } else if (index == 2 && (masks[0x3E] & 0x7C00)) {
            result = func_0025AA20(actor, state);
        }
        if (func_00219F74(actor, result, -1) == -1) {
            mode = 0x200000;
        }
    }
    if (mode & 0x200000) {
        if (index == 0 && (masks[0x3D] & 0x1F)) {
            result = func_0025A944(actor, state);
        } else if (index == 1 && (masks[0x3D] & 0x3E0)) {
            result = func_0025A970(actor, state);
        } else if (index == 2 && (masks[0x3D] & 0x7C00)) {
            result = func_0025A99C(actor, state);
        }
        if (func_00219F74(actor, result, -1) == -1) {
            mode = 0x100000;
        }
    }
    if (result == -1) {
        if (index == 0) {
            if ((masks[0x3C] & 0x1F) && state->anim >= 2000 && state->anim < 2025) {
                result = func_0025A8C0(actor, state);
            } else {
                result = func_0025A83C(actor, state);
            }
        } else if (index == 1) {
            if ((masks[0x3C] & 0x3E0) && state->anim >= 2100 && state->anim < 2125) {
                result = func_0025A8EC(actor, state);
            } else {
                result = func_0025A868(actor, state);
            }
        } else if (index == 2) {
            if ((masks[0x3C] & 0x7C00) && state->anim >= 2200 && state->anim < 2225) {
                result = func_0025A918(actor, state);
            } else {
                result = func_0025A894(actor, state);
            }
        }
    }
    state->flags = rest | mode;
    return result;
}
