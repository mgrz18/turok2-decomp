#include "common.h"
#include "structs/game_object.h"

extern f32 gQuaternionSin;

f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, f32 mtx[4][4]);
void CMtxF__ScaleAxes(f32 mtx[4][4], f32 x, f32 y, f32 z);
void CVector3__ClampLength(CVector3 *v, f32 limit);
void CMtxF__Translate(f32 mtx[4][4], f32 x, f32 y, f32 z);
void CMtxF__SwapYZRows(f32 mtx[4][4]);

void Turret_CalcOrientation(CGameObjectInstance *self, CAI *ai) {
    CQuaternion q;
    CQuaternion rot;
    f32 sinHalf;

    if (self->header.m_pIntelligence->m_Turret.m_RotNode == -1) {
        sinHalf = func_002982F0(self->m_RotY * 0.5f);
        q.x = 0.0f;
        q.y = sinHalf;
        q.z = 0.0f;
        gQuaternionSin = sinHalf;
        q.w = func_002974C0(self->m_RotY * 0.5f);
    } else {
        gQuaternionSin = func_002982F0(ai->m_InitialRotY * 0.5f);
    }
    q.x = 0.0f;
    q.y = gQuaternionSin;
    q.z = 0.0f;
    q.w = func_002974C0(ai->m_InitialRotY * 0.5f);
    CQuaternion__Multiply(&rot, &q, &self->m_qGround);
    CQuaternion__ToMatrix(&rot, self->m_mfOrient);
    CMtxF__ScaleAxes(self->m_mfOrient, self->m_vScale.x, self->m_vScale.y, self->m_vScale.z);
    CVector3__ClampLength(&self->header.m_vPos, 20000.0f);
    CMtxF__Translate(self->m_mfOrient, self->header.m_vPos.x, self->header.m_vPos.y, self->header.m_vPos.z);
    CMtxF__SwapYZRows(self->m_mfOrient);
}
