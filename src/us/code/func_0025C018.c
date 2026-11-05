#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"

/* AI matrix callback: builds the actors world matrix from the AIs look
 * angles, its yaw about Y, its rotation, scale and position. */

extern f32 D_800DD78C;
extern s32 D_8011AAD8;

extern void func_0025FEB0(CGameObjectInstance *actor, CAI *ai);
extern f32 func_0021170C(f32 angle, f32 step);
extern void func_00210630(CMtxF *m, f32 angle);
extern void func_00210790(CMtxF *m, f32 angle);
extern f32 func_002982F0(f32 x);
extern f32 func_002974C0(f32 x);
extern void func_00210F68(CQuaternion *out, CQuaternion *a, CQuaternion *b);
extern void func_00211114(CQuaternion *rot, CMtxF *m);
extern void func_0020C618(CMtxF *out, CMtxF *a, CMtxF *b);
extern void func_0021034C(CMtxF *m, f32 x, f32 y, f32 z);
extern void func_0020F510(CVector3 *v, f32 limit);
extern void func_00210318(CMtxF *m, f32 x, f32 y, f32 z);
extern void func_00210C3C(CMtxF *m);

void func_0025C018(CGameObjectInstance *actor, CAI *ai) {
    CMtxF look;
    CMtxF rotMtx;
    CQuaternion spin;
    CQuaternion rot;
    f32 sin;

    if (ai->m_ModeFlags & 0x10000) {
        func_0025FEB0(actor, ai);
    }
    if (ai->m_Health == 0) {
        ai->m_Enemy.m_Pitch = func_0021170C(ai->m_Enemy.m_Pitch, 0.0872664749f);
        ai->m_Enemy.m_Roll = func_0021170C(ai->m_Enemy.m_Roll, 0.0872664749f);
    }
    func_00210630(&look, ai->m_Enemy.m_Roll);
    func_00210790(&look, ai->m_Enemy.m_Pitch);
    sin = func_002982F0(actor->m_RotY * 0.5f);
    spin.x = 0.0f;
    spin.y = sin;
    spin.z = 0.0f;
    D_800DD78C = sin;
    spin.w = func_002974C0(actor->m_RotY * 0.5f);
    func_00210F68(&rot, &spin, &actor->m_qGround);
    func_00211114(&rot, &rotMtx);
    func_0020C618((CMtxF *) &actor->m_mfOrient, &look, &rotMtx);
    if (D_8011AAD8 & 0x40) {
        func_0021034C((CMtxF *) &actor->m_mfOrient, actor->m_vScale.x * 0.5f, actor->m_vScale.y * 0.5f, actor->m_vScale.z * 0.5f);
    } else {
        func_0021034C((CMtxF *) &actor->m_mfOrient, actor->m_vScale.x, actor->m_vScale.y, actor->m_vScale.z);
    }
    func_0020F510(&actor->header.m_vPos, 20000.0f);
    func_00210318((CMtxF *) &actor->m_mfOrient, actor->header.m_vPos.x, actor->header.m_vPos.y, actor->header.m_vPos.z);
    func_00210C3C((CMtxF *) &actor->m_mfOrient);
}
