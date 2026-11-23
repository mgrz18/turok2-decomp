#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/math.h"
#include "globals/ai.h"

/* Places a projectile at its owner's muzzle: inherits the shooter's rotation,
 * yaws it by its own angle and pushes it 10.24 units along the result. */


void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, CMtxF *mtx);
void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);
f32 func_002982F0(f32 x); /* sinf */
f32 func_002974C0(f32 x); /* cosf */

void Projectile_PlaceAtMuzzle(CGameObjectInstance *self, CAI *ai) {
    CMtxF m;
    CVector3 dir;
    CQuaternion yaw;
    CQuaternion rot;
    CGameObjectInstance *shooter = (CGameObjectInstance *) ai->m_Enemy.m_pGrabbed;
    CPlayer *owner = (CPlayer *) shooter->m_AI.m_pParent;
    f32 s;

    CVector3__Scale(&self->m_vScale, &owner->m_vOnEnemyScale, 0.30719998f);   /* LibTEngine's name; a velocity here */
    self->m_RotY = shooter->m_RotY;
    self->m_qGround = shooter->m_qGround;
    self->header.m_vPos = owner->m_pCamera->m_vPos;
    self->header.m_pCurrentRegion = (sptr) shooter->header.m_pCurrentRegion;
    s = func_002982F0(self->m_RotY * 0.5f);
    yaw.x = 0.0f;
    yaw.y = s;
    yaw.z = 0.0f;
    gQuaternionSin = s;
    yaw.w = func_002974C0(self->m_RotY * 0.5f);
    CQuaternion__Multiply(&rot, &yaw, &self->m_qGround);
    CQuaternion__ToMatrix(&rot, &m);
    CMtxF__TransformPoint(&m, &sProjectileMuzzleOffset, &dir);
    CVector3__Scale(&dir, &dir, 10.24f);
    CVector3__Add(&self->header.m_vPos, &self->header.m_vPos, &dir);
}
