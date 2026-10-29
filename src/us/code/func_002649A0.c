/* WRONG CUT: the function starts at 0x002649A0. The three words splat puts at
 * 0x00264994 (lw $2, 0x1CE0($2); beqz; lui $3, 0x100) are stray bytes, never
 * executed: the only reference, func_00264C94, stores func_00264994 + 0xC. */
#include "common.h"

extern f32 func_002671B4(void *obj);
extern void func_0020FC24(f32 *mtx, f32 x, f32 y, f32 z);
extern void func_00210968(f32 *mtx, f32 angle);
extern void func_00210790(f32 *mtx, f32 angle);
extern void func_00210318(f32 *mtx, f32 x, f32 y, f32 z);
extern f32 func_002982F0(f32 x); /* sinf */
extern f32 func_002974C0(f32 x); /* cosf */
extern void func_00210F68(f32 *out, f32 *a, f32 *b);
extern void func_00211114(f32 *quat, f32 *mtx);
extern void func_0020C618(f32 *out, f32 *left, f32 *right);
extern void func_0021034C(f32 *mtx, f32 x, f32 y, f32 z);
extern void func_0020F510(f32 *vec, f32 limit);
extern void func_00210C3C(f32 *mtx);

extern f32 D_800DD78C;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 pos[3];
    /* 0x10 */ u8 pad10[0x24];
    /* 0x34 */ f32 scale[3];
    /* 0x40 */ f32 rotation[4];
    /* 0x50 */ f32 angle;
    /* 0x54 */ u8 pad54[4];
    /* 0x58 */ f32 matrix[16];
} SpinObject;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ f32 tiltAngle;
    /* 0x114 */ f32 tiltHeading;
} SpinState;

void func_002649A0(SpinObject *obj, SpinState *state) {
    f32 rotMtx[16];
    f32 tiltMtx[16];
    f32 axis[4];
    f32 quat[4];
    f32 s;

    func_0020FC24(tiltMtx, 0.0f, -func_002671B4(obj) * 0.5f, 0.0f);
    func_00210968(tiltMtx, -state->tiltHeading);
    func_00210790(tiltMtx, state->tiltAngle);
    func_00210968(tiltMtx, state->tiltHeading);
    func_00210318(tiltMtx, 0.0f, func_002671B4(obj) * 0.5f, 0.0f);

    s = func_002982F0(obj->angle * 0.5f);
    axis[0] = 0.0f;
    axis[1] = s;
    axis[2] = 0.0f;
    D_800DD78C = s;
    axis[3] = func_002974C0(obj->angle * 0.5f);
    func_00210F68(quat, axis, obj->rotation);
    func_00211114(quat, rotMtx);

    func_0020C618(obj->matrix, tiltMtx, rotMtx);
    func_0021034C(obj->matrix, obj->scale[0], obj->scale[1], obj->scale[2]);
    func_0020F510(obj->pos, 20000.0f);
    func_00210318(obj->matrix, obj->pos[0], obj->pos[1], obj->pos[2]);
    func_00210C3C(obj->matrix);
}
