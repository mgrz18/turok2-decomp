#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ s32 spinMode;
} Def;


typedef struct {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ f32 spin;
} AI;

extern f32 D_800DD78C;

f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_00210F68(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void func_00211114(CQuaternion *q, f32 mtx[4][4]);
void func_0021034C(f32 mtx[4][4], f32 x, f32 y, f32 z);
void func_0020F510(CVector3 *v, f32 limit);
void func_00210318(f32 mtx[4][4], f32 x, f32 y, f32 z);
void func_00210C3C(f32 mtx[4][4]);

void func_00262A1C(CGameObjectInstance *self, AI *ai) {
    CQuaternion q;
    CQuaternion rot;
    f32 sinHalf;

    if (((Def *)self->header.m_pIntelligence)->spinMode == -1) {
        sinHalf = func_002982F0(self->m_RotY * 0.5f);
        q.x = 0.0f;
        q.y = sinHalf;
        q.z = 0.0f;
        D_800DD78C = sinHalf;
        q.w = func_002974C0(self->m_RotY * 0.5f);
    } else {
        D_800DD78C = func_002982F0(ai->spin * 0.5f);
    }
    q.x = 0.0f;
    q.y = D_800DD78C;
    q.z = 0.0f;
    q.w = func_002974C0(ai->spin * 0.5f);
    func_00210F68(&rot, &q, &self->m_qGround);
    func_00211114(&rot, self->m_mfOrient);
    func_0021034C(self->m_mfOrient, self->m_vScale.x, self->m_vScale.y, self->m_vScale.z);
    func_0020F510(&self->header.m_vPos, 20000.0f);
    func_00210318(self->m_mfOrient, self->header.m_vPos.x, self->header.m_vPos.y, self->header.m_vPos.z);
    func_00210C3C(self->m_mfOrient);
}
