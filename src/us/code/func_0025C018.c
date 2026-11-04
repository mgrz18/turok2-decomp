#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"

/* AI matrix callback: builds the actors world matrix from the AIs look
 * angles, its yaw about Y, its rotation, scale and position. */

typedef struct { f32 m[4][4]; } Mtx44;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[0x34 - 0x10];
    /* 0x34 */ CVector3 scale;
    /* 0x40 */ CQuaternion rot;
    /* 0x50 */ f32 yaw;
    /* 0x54 */ u8 pad54[0x4];
    /* 0x58 */ Mtx44 mtx;
} Actor;

extern f32 D_800DD78C;
extern s32 D_8011AAD8;

extern void func_0025FEB0(Actor *actor, CAI *ai);
extern f32 func_0021170C(f32 angle, f32 step);
extern void func_00210630(Mtx44 *m, f32 angle);
extern void func_00210790(Mtx44 *m, f32 angle);
extern f32 func_002982F0(f32 x);
extern f32 func_002974C0(f32 x);
extern void func_00210F68(CQuaternion *out, CQuaternion *a, CQuaternion *b);
extern void func_00211114(CQuaternion *rot, Mtx44 *m);
extern void func_0020C618(Mtx44 *out, Mtx44 *a, Mtx44 *b);
extern void func_0021034C(Mtx44 *m, f32 x, f32 y, f32 z);
extern void func_0020F510(CVector3 *v, f32 limit);
extern void func_00210318(Mtx44 *m, f32 x, f32 y, f32 z);
extern void func_00210C3C(Mtx44 *m);

void func_0025C018(Actor *actor, CAI *ai) {
    Mtx44 look;
    Mtx44 rotMtx;
    CQuaternion spin;
    CQuaternion rot;
    f32 sin;

    if (ai->m_ModeFlags & 0x10000) {
        func_0025FEB0(actor, ai);
    }
    if (ai->m_Health == 0) {
        ai->m_VarF32[0] = func_0021170C(ai->m_VarF32[0], 0.0872664749f);
        ai->m_VarF32[1] = func_0021170C(ai->m_VarF32[1], 0.0872664749f);
    }
    func_00210630(&look, ai->m_VarF32[1]);
    func_00210790(&look, ai->m_VarF32[0]);
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
