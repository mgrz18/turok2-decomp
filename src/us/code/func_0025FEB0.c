#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"

/* Places a projectile at its owner's muzzle: inherits the shooter's rotation,
 * yaws it by its own angle and pushes it 10.24 units along the result. */

typedef struct {
    f32 m[4][4];
} Mtx4;

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ CVector3 muzzle;
} Weapon;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ Weapon *weapon;
    /* 0x520 */ u8 pad520[0xBB8 - 0x520];
    /* 0xBB8 */ CVector3 vel;
} Owner;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x20];
    /* 0x34 */ CVector3 vel;
    /* 0x40 */ CQuaternion rot;
    /* 0x50 */ f32 yaw;
} Projectile;

typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ s32 unk10;
    /* 0x014 */ u8 pad14[0x2C];
    /* 0x040 */ CQuaternion rot;
    /* 0x050 */ f32 yaw;
    /* 0x054 */ u8 pad54[0x1A8 - 0x54];
    /* 0x1A8 */ Owner *owner;
} Shooter;

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
    Shooter *shooter = ai->m_VarPtr[2];
    Owner *owner = shooter->owner;
    f32 s;

    func_0020EF60(&self->vel, &owner->vel, 0.30719998f);
    self->yaw = shooter->yaw;
    self->rot = shooter->rot;
    self->pos = owner->weapon->muzzle;
    self->unk10 = shooter->unk10;
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
