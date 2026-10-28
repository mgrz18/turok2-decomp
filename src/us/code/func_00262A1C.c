#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    f32 x, y, z, w;
} Quat;

typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ s32 spinMode;
} Def;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ Def *def;
    /* 0x18 */ u8 pad18[0x34 - 0x18];
    /* 0x34 */ Vec3 scale;
    /* 0x40 */ Quat rotation;
    /* 0x50 */ f32 rotY;
    /* 0x54 */ u8 pad54[0x58 - 0x54];
    /* 0x58 */ f32 mtx[4][4];
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ f32 spin;
} AI;

extern f32 D_800DD78C;

f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_00210F68(Quat *out, Quat *a, Quat *b);
void func_00211114(Quat *q, f32 mtx[4][4]);
void func_0021034C(f32 mtx[4][4], f32 x, f32 y, f32 z);
void func_0020F510(Vec3 *v, f32 limit);
void func_00210318(f32 mtx[4][4], f32 x, f32 y, f32 z);
void func_00210C3C(f32 mtx[4][4]);

void func_00262A1C(Actor *self, AI *ai) {
    Quat q;
    Quat rot;
    f32 sinHalf;

    if (self->def->spinMode == -1) {
        sinHalf = func_002982F0(self->rotY * 0.5f);
        q.x = 0.0f;
        q.y = sinHalf;
        q.z = 0.0f;
        D_800DD78C = sinHalf;
        q.w = func_002974C0(self->rotY * 0.5f);
    } else {
        D_800DD78C = func_002982F0(ai->spin * 0.5f);
    }
    q.x = 0.0f;
    q.y = D_800DD78C;
    q.z = 0.0f;
    q.w = func_002974C0(ai->spin * 0.5f);
    func_00210F68(&rot, &q, &self->rotation);
    func_00211114(&rot, self->mtx);
    func_0021034C(self->mtx, self->scale.x, self->scale.y, self->scale.z);
    func_0020F510(&self->pos, 20000.0f);
    func_00210318(self->mtx, self->pos.x, self->pos.y, self->pos.z);
    func_00210C3C(self->mtx);
}
