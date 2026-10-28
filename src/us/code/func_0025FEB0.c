#include "common.h"

/* Places a projectile at its owner's muzzle: inherits the shooter's rotation,
 * yaws it by its own angle and pushes it 10.24 units along the result. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 x, y, z, w;
} Quat;

typedef struct {
    f32 m[4][4];
} Mtx4;

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ Vec3f muzzle;
} Weapon;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ Weapon *weapon;
    /* 0x520 */ u8 pad520[0xBB8 - 0x520];
    /* 0xBB8 */ Vec3f vel;
} Owner;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[0x20];
    /* 0x34 */ Vec3f vel;
    /* 0x40 */ Quat rot;
    /* 0x50 */ f32 yaw;
} Projectile;

typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ s32 unk10;
    /* 0x014 */ u8 pad14[0x2C];
    /* 0x040 */ Quat rot;
    /* 0x050 */ f32 yaw;
    /* 0x054 */ u8 pad54[0x1A8 - 0x54];
    /* 0x1A8 */ Owner *owner;
} Shooter;

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ Shooter *shooter;
} AI;

extern Vec3f D_800B17B0;
extern f32 D_800DD78C;
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);
void func_00210F68(Quat *out, Quat *a, Quat *b);
void func_00211114(Quat *q, Mtx4 *mtx);
void func_0020F85C(Mtx4 *m, Vec3f *in, Vec3f *out);
f32 func_002982F0(f32 x); /* sinf */
f32 func_002974C0(f32 x); /* cosf */

void func_0025FEB0(Projectile *self, AI *ai) {
    Mtx4 m;
    Vec3f dir;
    Quat yaw;
    Quat rot;
    Shooter *shooter = ai->shooter;
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
