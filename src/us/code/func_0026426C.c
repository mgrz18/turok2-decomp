#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    f32 x, y, z, w;
} Quat;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ u8 pad10[0x34 - 0x10];
    /* 0x34 */ Vec3 scale;
    /* 0x40 */ Quat rotation;
    /* 0x50 */ f32 rotY;
    /* 0x54 */ u8 pad54[0x58 - 0x54];
    /* 0x58 */ f32 mtx[4][4];
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x11C];
    /* 0x11C */ f32 tiltZ;
    /* 0x120 */ f32 tiltX;
} AI;

extern f32 D_800DD78C;

void func_00210630(f32 mtx[4][4], f32 angle);
void func_00210790(f32 mtx[4][4], f32 angle);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_00210F68(Quat *out, Quat *a, Quat *b);
void func_00211114(Quat *q, f32 mtx[4][4]);
void func_0020C618(f32 out[4][4], f32 a[4][4], f32 b[4][4]);
void func_0021034C(f32 mtx[4][4], f32 x, f32 y, f32 z);
void func_0020F510(Vec3 *v, f32 limit);
void func_00210318(f32 mtx[4][4], f32 x, f32 y, f32 z);
void func_00210C3C(f32 mtx[4][4]);

void func_0026426C(Actor *self, AI *ai) {
    f32 tilt[4][4];
    f32 spin[4][4];
    Quat q;
    Quat rot;
    f32 sinHalf;

    func_00210630(tilt, ai->tiltX);
    func_00210790(tilt, ai->tiltZ);
    sinHalf = func_002982F0(self->rotY * 0.5f);
    q.x = 0.0f;
    q.y = sinHalf;
    q.z = 0.0f;
    D_800DD78C = sinHalf;
    q.w = func_002974C0(self->rotY * 0.5f);
    func_00210F68(&rot, &q, &self->rotation);
    func_00211114(&rot, spin);
    func_0020C618(self->mtx, tilt, spin);
    func_0021034C(self->mtx, self->scale.x, self->scale.y, self->scale.z);
    func_0020F510(&self->pos, 20000.0f);
    func_00210318(self->mtx, self->pos.x, self->pos.y, self->pos.z);
    func_00210C3C(self->mtx);
}
