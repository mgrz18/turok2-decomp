#include "common.h"

/* AI matrix callback: builds the actors world matrix from the AIs look
 * angles, its yaw about Y, its rotation, scale and position. */

typedef struct { f32 x, y, z; } Vec3;
typedef struct { f32 x, y, z, w; } Quat;
typedef struct { f32 m[4][4]; } Mtx44;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ u8 pad10[0x34 - 0x10];
    /* 0x34 */ Vec3 scale;
    /* 0x40 */ Quat rot;
    /* 0x50 */ f32 yaw;
    /* 0x54 */ u8 pad54[0x4];
    /* 0x58 */ Mtx44 mtx;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ s32 unk4;
    /* 0x008 */ u8 pad8[0x3C - 0x8];
    /* 0x03C */ u32 flags;
    /* 0x040 */ u8 pad40[0x10C - 0x40];
    /* 0x10C */ f32 pitch;
    /* 0x110 */ f32 roll;
} CAI;

extern f32 D_800DD78C;
extern s32 D_8011AAD8;

extern void func_0025FEB0(Actor *actor, CAI *ai);
extern f32 func_0021170C(f32 angle, f32 step);
extern void func_00210630(Mtx44 *m, f32 angle);
extern void func_00210790(Mtx44 *m, f32 angle);
extern f32 func_002982F0(f32 x);
extern f32 func_002974C0(f32 x);
extern void func_00210F68(Quat *out, Quat *a, Quat *b);
extern void func_00211114(Quat *rot, Mtx44 *m);
extern void func_0020C618(Mtx44 *out, Mtx44 *a, Mtx44 *b);
extern void func_0021034C(Mtx44 *m, f32 x, f32 y, f32 z);
extern void func_0020F510(Vec3 *v, f32 limit);
extern void func_00210318(Mtx44 *m, f32 x, f32 y, f32 z);
extern void func_00210C3C(Mtx44 *m);

void func_0025C018(Actor *actor, CAI *ai) {
    Mtx44 look;
    Mtx44 rotMtx;
    Quat spin;
    Quat rot;
    f32 sin;

    if (ai->flags & 0x10000) {
        func_0025FEB0(actor, ai);
    }
    if (ai->unk4 == 0) {
        ai->pitch = func_0021170C(ai->pitch, 0.0872664749f);
        ai->roll = func_0021170C(ai->roll, 0.0872664749f);
    }
    func_00210630(&look, ai->roll);
    func_00210790(&look, ai->pitch);
    sin = func_002982F0(actor->yaw * 0.5f);
    spin.x = 0.0f;
    spin.y = sin;
    spin.z = 0.0f;
    D_800DD78C = sin;
    spin.w = func_002974C0(actor->yaw * 0.5f);
    func_00210F68(&rot, &spin, &actor->rot);
    func_00211114(&rot, &rotMtx);
    func_0020C618(&actor->mtx, &look, &rotMtx);
    if (D_8011AAD8 & 0x40) {
        func_0021034C(&actor->mtx, actor->scale.x * 0.5f, actor->scale.y * 0.5f, actor->scale.z * 0.5f);
    } else {
        func_0021034C(&actor->mtx, actor->scale.x, actor->scale.y, actor->scale.z);
    }
    func_0020F510(&actor->pos, 20000.0f);
    func_00210318(&actor->mtx, actor->pos.x, actor->pos.y, actor->pos.z);
    func_00210C3C(&actor->mtx);
}
