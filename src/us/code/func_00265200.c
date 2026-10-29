/* Boundary fixed in versions/cuts.us.txt: the function starts at 0x00265200 (its pointer in the data is
 * 0x00265200). The word splat puts at 0x002651FC, `jal func_0022F1C8`, is a
 * stray word ahead of the prologue, never executed. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0020486C.c, func_0020486C, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    s32 a;
    s32 b;
} EventArg;

typedef struct {
    /* 0x0 */ u32 flags;
    /* 0x4 */ f32 duration;
    /* 0x8 */ s8 eventFrame; /* in 1/127ths of the animation, -1: none */
} ActionInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActionInfo info;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xA2];
    /* 0x0BA */ s16 unkBA;
    /* 0x0BC */ u8 padBC[0x18];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ f32 animFrame;
    /* 0x0DC */ s16 anim;
    /* 0x0DE */ s16 nextAnim;
    /* 0x0E0 */ s16 animLength;
    /* 0x0E2 */ u8 padE2[0x1E];
    /* 0x100 */ s16 blendLength;
    /* 0x102 */ u8 pad102[6];
    /* 0x108 */ f32 blendFrame;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 timer;
    /* 0x44 */ u8 pad44[0x20];
    /* 0x64 */ f32 duration;
    /* 0x68 */ u8 pad68[0x5E];
    /* 0xC6 */ u8 anim;
    /* 0xC7 */ s8 active;
} ActionState;

extern u8 D_800F7078[];
extern void func_00237EE4(Actor *a, Actor *b, s32 kind, Vec3f pos, EventArg extra);
extern void func_00220260(void *mgr, Actor *actor, s32 flag);
extern void func_00243414(Actor *actor, ActionState *state, s32 mode);

static __inline__ s32 animReached(Actor *actor, ActionState *state, f32 scale) {
    if (actor->unkBA == 1) return 1;
    if (actor->anim != (s8) state->anim || actor->nextAnim != (s8) state->anim) return 0;
    if (state->active != 0 && !(actor->flags & 0x400)) return 1;
    if (actor->animLength < 3 && (actor->flags & 0x400)) {
        return (f32) actor->blendLength * scale <= actor->blendFrame;
    } else {
        return (f32) actor->animLength * scale <= actor->animFrame;
    }
}

void func_00265200(Actor *actor, ActionState *state) {
    ActionInfo *info = &actor->def->info;
    EventArg extra;
    f32 t;

    if (info->eventFrame != -1) {
        if (animReached(actor, state, (f32) info->eventFrame * 0.007874016f)) {
            extra.a = 0;
            func_00237EE4(actor, actor, 6, actor->pos, extra);
            func_00220260(D_800F7078, actor, 1);
        }
    }
    if ((info->flags & 1) && state->active != 0) {
        t = info->duration;
        if (t == 0.0f || state->duration < t) {
            if (state->duration == 0.0f) {
                state->timer = 0.0f;
            }
            state->duration = t;
        }
    }
    if (state->duration != 0.0f && state->duration <= state->timer) {
        func_00243414(actor, state, 1);
    }
}
