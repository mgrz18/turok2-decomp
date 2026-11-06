#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"

/* AI matrix callback: builds the actors world matrix from the AIs look
 * angles, its yaw about Y, its rotation, scale and position. */

extern f32 gQuaternionSin;
extern s32 gEngineApp_m_Settings;

extern void Projectile_PlaceAtMuzzle(CGameObjectInstance *actor, CAI *ai);
extern f32 Math_DecayToZero(f32 angle, f32 step);
extern void CMtxF__SetRotateZ(CMtxF *m, f32 angle);
extern void CMtxF__RotateX(CMtxF *m, f32 angle);
extern f32 func_002982F0(f32 x);
extern f32 func_002974C0(f32 x);
extern void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
extern void CQuaternion__ToMatrix(CQuaternion *rot, CMtxF *m);
extern void CMtxF__MultiplyAffine(CMtxF *out, CMtxF *a, CMtxF *b);
extern void CMtxF__ScaleAxes(CMtxF *m, f32 x, f32 y, f32 z);
extern void CVector3__ClampLength(CVector3 *v, f32 limit);
extern void CMtxF__Translate(CMtxF *m, f32 x, f32 y, f32 z);
extern void CMtxF__SwapYZRows(CMtxF *m);

void EnemyAI_CalcOrientationMatrix(CGameObjectInstance *actor, CAI *ai) {
    CMtxF look;
    CMtxF rotMtx;
    CQuaternion spin;
    CQuaternion rot;
    f32 sin;

    if (ai->m_ModeFlags & 0x10000) {
        Projectile_PlaceAtMuzzle(actor, ai);
    }
    if (ai->m_Health == 0) {
        ai->m_Enemy.m_Pitch = Math_DecayToZero(ai->m_Enemy.m_Pitch, 0.0872664749f);
        ai->m_Enemy.m_Roll = Math_DecayToZero(ai->m_Enemy.m_Roll, 0.0872664749f);
    }
    CMtxF__SetRotateZ(&look, ai->m_Enemy.m_Roll);
    CMtxF__RotateX(&look, ai->m_Enemy.m_Pitch);
    sin = func_002982F0(actor->m_RotY * 0.5f);
    spin.x = 0.0f;
    spin.y = sin;
    spin.z = 0.0f;
    gQuaternionSin = sin;
    spin.w = func_002974C0(actor->m_RotY * 0.5f);
    CQuaternion__Multiply(&rot, &spin, &actor->m_qGround);
    CQuaternion__ToMatrix(&rot, &rotMtx);
    CMtxF__MultiplyAffine((CMtxF *) &actor->m_mfOrient, &look, &rotMtx);
    if (gEngineApp_m_Settings & 0x40) {
        CMtxF__ScaleAxes((CMtxF *) &actor->m_mfOrient, actor->m_vScale.x * 0.5f, actor->m_vScale.y * 0.5f, actor->m_vScale.z * 0.5f);
    } else {
        CMtxF__ScaleAxes((CMtxF *) &actor->m_mfOrient, actor->m_vScale.x, actor->m_vScale.y, actor->m_vScale.z);
    }
    CVector3__ClampLength(&actor->header.m_vPos, 20000.0f);
    CMtxF__Translate((CMtxF *) &actor->m_mfOrient, actor->header.m_vPos.x, actor->header.m_vPos.y, actor->header.m_vPos.z);
    CMtxF__SwapYZRows((CMtxF *) &actor->m_mfOrient);
}
