/* WRONG CUT: the function starts at 0x002649A0. The three words splat puts at
 * 0x00264994 (lw $2, 0x1CE0($2); beqz; lui $3, 0x100) are stray bytes, never
 * executed: the only reference, Throwable_Setup, stores func_00264994 + 0xC. */
#include "common.h"
#include "globals/math.h"

extern f32 GameObject_GetCollisionHeight(void *obj);
extern void CMtxF__SetTranslate(f32 *mtx, f32 x, f32 y, f32 z);
extern void CMtxF__RotateY(f32 *mtx, f32 angle);
extern void CMtxF__RotateX(f32 *mtx, f32 angle);
extern void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
extern f32 func_002982F0(f32 x); /* sinf */
extern f32 func_002974C0(f32 x); /* cosf */
extern void CQuaternion__Multiply(f32 *out, f32 *a, f32 *b);
extern void CQuaternion__ToMatrix(f32 *quat, f32 *mtx);
extern void CMtxF__MultiplyAffine(f32 *out, f32 *left, f32 *right);
extern void CMtxF__ScaleAxes(f32 *mtx, f32 x, f32 y, f32 z);
extern void CVector3__ClampLength(f32 *vec, f32 limit);
extern void CMtxF__SwapYZRows(f32 *mtx);


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

void Throwable_CalcOrientation(SpinObject *obj, SpinState *state) {
    f32 rotMtx[16];
    f32 tiltMtx[16];
    f32 axis[4];
    f32 quat[4];
    f32 s;

    CMtxF__SetTranslate(tiltMtx, 0.0f, -GameObject_GetCollisionHeight(obj) * 0.5f, 0.0f);
    CMtxF__RotateY(tiltMtx, -state->tiltHeading);
    CMtxF__RotateX(tiltMtx, state->tiltAngle);
    CMtxF__RotateY(tiltMtx, state->tiltHeading);
    CMtxF__Translate(tiltMtx, 0.0f, GameObject_GetCollisionHeight(obj) * 0.5f, 0.0f);

    s = func_002982F0(obj->angle * 0.5f);
    axis[0] = 0.0f;
    axis[1] = s;
    axis[2] = 0.0f;
    gQuaternionSin = s;
    axis[3] = func_002974C0(obj->angle * 0.5f);
    CQuaternion__Multiply(quat, axis, obj->rotation);
    CQuaternion__ToMatrix(quat, rotMtx);

    CMtxF__MultiplyAffine(obj->matrix, tiltMtx, rotMtx);
    CMtxF__ScaleAxes(obj->matrix, obj->scale[0], obj->scale[1], obj->scale[2]);
    CVector3__ClampLength(obj->pos, 20000.0f);
    CMtxF__Translate(obj->matrix, obj->pos[0], obj->pos[1], obj->pos[2]);
    CMtxF__SwapYZRows(obj->matrix);
}
