#include "common.h"

extern void func_00264B0C(void *obj, void *state, f32 *out);
extern s32 func_00219F74(void *actor, s32 action, s32 skip);
extern void func_0020EF8C(f32 *out, f32 t, f32 *from, f32 *to);
extern void func_00243414(void *obj, void *state, s32 mode);

extern f32 D_800B6D28; /* frame time scale */
extern s8 D_800F1F94;

typedef struct {
    /* 0x000 */ u8 pad0[0x50];
    /* 0x050 */ f32 angle;
    /* 0x054 */ u8 pad54[0x84];
    /* 0x0D8 */ f32 animFrame;
    /* 0x0DC */ s16 anim;
    /* 0x0DE */ u8 padDE[2];
    /* 0x0E0 */ s16 animLength;
    /* 0x0E2 */ u8 padE2[0x92];
    /* 0x174 */ s8 mode;
} OwnerActor;

typedef struct {
    /* 0x000 */ u8 pad0[0x54];
    /* 0x054 */ f32 pos[3];
    /* 0x060 */ u8 pad60[4];
    /* 0x064 */ f32 blend;
    /* 0x068 */ u8 pad68[0xA4];
    /* 0x10C */ OwnerActor *owner;
    /* 0x110 */ f32 tiltAngle;
    /* 0x114 */ f32 tiltHeading;
} OwnedState;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 pos[3];
    /* 0x10 */ u8 pad10[0x40];
    /* 0x50 */ f32 angle;
} Object;

void func_00264DF0(Object *obj, OwnedState *state) {
    f32 target[3];
    s32 anim;
    OwnerActor *owner = state->owner;

    D_800F1F94 = 0;
    if (owner == NULL) {
        return;
    }
    anim = func_00219F74(owner, 10000, -1);
    if (owner->anim == anim
        && owner->animLength * 0.3f <= owner->animFrame) {
        func_00264B0C(obj, state, target);
        state->tiltHeading = owner->angle - obj->angle;
        state->blend += D_800B6D28 * 0.25f;
        if (state->blend > 1.0f) {
            state->blend = 1.0f;
        }
        func_0020EF8C(state->pos, state->blend, obj->pos, target);
        if (owner->animFrame < owner->animLength * 0.5f) {
            state->tiltAngle += 0.087266475f;
        } else {
            state->tiltAngle -= 0.087266475f;
        }
    }
    if (owner != NULL && owner->mode != 0x15) {
        func_00243414(obj, state, 2);
    }
}
