#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/math.h"
#include "globals/ai.h"

/* Places a projectile at its owner's muzzle: inherits the shooter's rotation,
 * yaws it by its own angle and pushes it 10.24 units along the result. */

#ifdef TARGET_PORT
typedef struct Projectile {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Projectile(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Projectile(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_unk10[T2_HOFF_Projectile(header.m_pCurrentRegion)]; sptr unk10; }; /* N64 0x10 */
        struct { u8 _hp_vel[T2_HOFF_Projectile(m_vScale)]; CVector3 vel; }; /* N64 0x34 */
        struct { u8 _hp_rot[T2_HOFF_Projectile(m_qGround)]; CQuaternion rot; }; /* N64 0x40 */
        struct { u8 _hp_yaw[T2_HOFF_Projectile(m_RotY)]; f32 yaw; }; /* N64 0x50 */
    };
} Projectile;
#else
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x20];
    /* 0x34 */ CVector3 vel;
    /* 0x40 */ CQuaternion rot;
    /* 0x50 */ f32 yaw;
} Projectile;
#endif

void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, CMtxF *mtx);
void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);
f32 func_002982F0(f32 x); /* sinf */
f32 func_002974C0(f32 x); /* cosf */

void Projectile_PlaceAtMuzzle(Projectile *self, CAI *ai) {
    CMtxF m;
    CVector3 dir;
    CQuaternion yaw;
    CQuaternion rot;
    CGameObjectInstance *shooter = (CGameObjectInstance *) ai->m_Enemy.m_pGrabbed;
    CPlayer *owner = (CPlayer *) shooter->m_AI.m_pParent;
    f32 s;

    CVector3__Scale(&self->vel, &owner->m_vOnEnemyScale, 0.30719998f);   /* LibTEngine's name; a velocity here */
    self->yaw = shooter->m_RotY;
    self->rot = shooter->m_qGround;
    self->pos = owner->m_pCamera->m_vPos;
    self->unk10 = (sptr) shooter->header.m_pCurrentRegion;
    s = func_002982F0(self->yaw * 0.5f);
    yaw.x = 0.0f;
    yaw.y = s;
    yaw.z = 0.0f;
    gQuaternionSin = s;
    yaw.w = func_002974C0(self->yaw * 0.5f);
    CQuaternion__Multiply(&rot, &yaw, &self->rot);
    CQuaternion__ToMatrix(&rot, &m);
    CMtxF__TransformPoint(&m, &sProjectileMuzzleOffset, &dir);
    CVector3__Scale(&dir, &dir, 10.24f);
    CVector3__Add(&self->pos, &self->pos, &dir);
}
