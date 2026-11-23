/* WRONG CUT: the function starts at 0x002649A0. The three words splat puts at
 * 0x00264994 (lw $2, 0x1CE0($2); beqz; lui $3, 0x100) are stray bytes, never
 * executed: the only reference, Throwable_Setup, stores func_00264994 + 0xC. */
#include "common.h"
#include "globals/math.h"
#include "structs/game_object.h"

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



void Throwable_CalcOrientation(CGameObjectInstance *obj, CAI *state) {
    f32 rotMtx[16];
    f32 tiltMtx[16];
    f32 axis[4];
    f32 quat[4];
    f32 s;

    CMtxF__SetTranslate(tiltMtx, 0.0f, -GameObject_GetCollisionHeight(obj) * 0.5f, 0.0f);
    CMtxF__RotateY(tiltMtx, -state->m_Throwable.m_TiltHeading);
    CMtxF__RotateX(tiltMtx, state->m_Throwable.m_TiltAngle);
    CMtxF__RotateY(tiltMtx, state->m_Throwable.m_TiltHeading);
    CMtxF__Translate(tiltMtx, 0.0f, GameObject_GetCollisionHeight(obj) * 0.5f, 0.0f);

    s = func_002982F0(obj->m_RotY * 0.5f);
    axis[0] = 0.0f;
    axis[1] = s;
    axis[2] = 0.0f;
    gQuaternionSin = s;
    axis[3] = func_002974C0(obj->m_RotY * 0.5f);
    CQuaternion__Multiply(quat, axis, &obj->m_qGround.x);
    CQuaternion__ToMatrix(quat, rotMtx);

    CMtxF__MultiplyAffine(obj->m_mfOrient[0], tiltMtx, rotMtx);
    CMtxF__ScaleAxes(obj->m_mfOrient[0], obj->m_vScale.x, obj->m_vScale.y, obj->m_vScale.z);
    CVector3__ClampLength(&obj->header.m_vPos.x, 20000.0f);
    CMtxF__Translate(obj->m_mfOrient[0], obj->header.m_vPos.x, obj->header.m_vPos.y, obj->header.m_vPos.z);
    CMtxF__SwapYZRows(obj->m_mfOrient[0]);
}
