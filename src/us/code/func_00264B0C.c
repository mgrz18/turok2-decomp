#include "common.h"

extern void func_0020D908(f32 *out, f32 *mtx);
extern void func_0020FC24(f32 *mtx, f32 x, f32 y, f32 z);
extern void func_00210318(f32 *mtx, f32 x, f32 y, f32 z);
extern void func_0020F85C(f32 *mtx, f32 *in, f32 *out);
extern f32 func_002671B4(void *obj);

typedef struct {
    /* 0x0 */ s32 joint;
    /* 0x4 */ f32 offset[3];
} JointPoint;

extern JointPoint D_800B21C0;
extern JointPoint D_800B21D0;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 pos[3];
    /* 0x10 */ u8 pad10[0x88];
    /* 0x98 */ u8 *jointMatrices; /* 0x40 bytes each */
} OwnerActor;

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ OwnerActor *owner;
} OwnedState;

/* Midpoint of two attachment points on the owner, lifted by the height. */
void func_00264B0C(void *obj, OwnedState *state, f32 *out) {
    f32 mtxA[16];
    f32 mtxB[16];
    f32 a[4];
    f32 b[4];
    JointPoint *pa = &D_800B21C0;
    JointPoint *pb = &D_800B21D0;
    OwnerActor *owner = state->owner;
    OwnerActor *src = owner;

    if (owner->jointMatrices != NULL) {
        func_0020D908(mtxA, (f32 *) (owner->jointMatrices + (pa->joint << 6)));
        func_0020D908(mtxB, (f32 *) (owner->jointMatrices + (pb->joint << 6)));
    } else {
        func_0020FC24(mtxA, src->pos[0], src->pos[1], src->pos[2]);
        func_0020FC24(mtxB, src->pos[0], src->pos[1], src->pos[2]);
    }
    func_00210318(mtxA, 0.0f, func_002671B4(obj), 0.0f);
    func_00210318(mtxB, 0.0f, func_002671B4(obj), 0.0f);
    func_0020F85C(mtxA, pa->offset, a);
    func_0020F85C(mtxB, pb->offset, b);
    out[0] = (a[0] + b[0]) * 0.5f;
    out[1] = (a[1] + b[1]) * 0.5f;
    out[2] = (a[2] + b[2]) * 0.5f;
}
