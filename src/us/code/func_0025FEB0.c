#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/camera.h"

/* Places a projectile at its owner's muzzle: inherits the shooter's rotation,
 * yaws it by its own angle and pushes it 10.24 units along the result. */

typedef struct {
    f32 m[4][4];
} Mtx4;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x20];
    /* 0x34 */ CVector3 vel;
    /* 0x40 */ CQuaternion rot;
    /* 0x50 */ f32 yaw;
} Projectile;

extern CVector3 D_800B17B0;
extern f32 D_800DD78C;
void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);
void func_00210F68(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void func_00211114(CQuaternion *q, Mtx4 *mtx);
void func_0020F85C(Mtx4 *m, CVector3 *in, CVector3 *out);
f32 func_002982F0(f32 x); /* sinf */
f32 func_002974C0(f32 x); /* cosf */

void func_0025FEB0(Projectile *self, CAI *ai) {
    Mtx4 m;
    CVector3 dir;
    CQuaternion yaw;
    CQuaternion rot;
    CGameObjectInstance *shooter = (CGameObjectInstance *) ai->m_Enemy.m_pGrabbed;
    CPlayer *owner = (CPlayer *) shooter->m_AI.m_pParent;
    f32 s;

    func_0020EF60(&self->vel, &owner->m_vOnEnemyScale, 0.30719998f);   /* LibTEngine's name; a velocity here */
    self->yaw = shooter->m_RotY;
    self->rot = shooter->m_qGround;
    self->pos = owner->m_pCamera->m_vPos;
    self->unk10 = (s32) shooter->header.m_pCurrentRegion;
    s = func_002982F0(self->yaw * 0.5f);
    yaw.x = 0.0f;
    yaw.y = s;
    yaw.z = 0.0f;
    D_800DD78C = s;
    yaw.w = func_002974C0(self->yaw * 0.5f);
    func_00210F68(&rot, &yaw, &self->rot);
    func_00211114(&rot, &m);
    func_0020F85C(&m, &D_800B17B0, &dir);
    func_0020EF60(&dir, &dir, 10.24f);
    func_0020EEF8(&self->pos, &self->pos, &dir);
}
