#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ CGameObjectInstance *obj;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 pad0C[0x88 - 0x0C];
} Targets;

/* Boss module state from 0x8012F9E4. */
typedef struct {
    /* 0x00 */ f32 timer;           /* 0x8012F9E4 */
    /* 0x04 */ f32 pushTimer;       /* 0x8012F9E8 */
    /* 0x08 */ u8 pad08[0x18];
    /* 0x20 */ s32 index;           /* 0x8012FA04 */
} BossTimers;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00245BAC(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a - b */
extern f32 func_0020EAA0(CVector3 *v);                                    /* heading */
extern s32 func_002685F0(CGameObjectInstance *obj);
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */

extern s32 D_800F1F50[];
extern f32 D_800B6D28;          /* per-frame scale */
extern f32 D_8012F628;          /* last angle to the target */
extern BossTimers D_8012F9E4;

/* Pushes the target away from the boss while the push timer has run out. */
void func_0040688C(CGameObjectInstance *obj, CAI *ai) {
    Targets t;
    CVector3 pos;
    CVector3 d;
    CGameObjectInstance *target;
    f32 heading;
    f32 push;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 0, 1);
    D_800F1F50[0] &= ~0x8000;
    if (t.obj != NULL) {
        func_00245BAC(obj, ai, 0.261799425f, t.angle);
        D_8012F628 = t.angle;
    } else {
        func_00245BAC(obj, ai, 0.261799425f, D_8012F628);
    }
    target = t.obj;
    if (target != NULL && D_8012F9E4.pushTimer <= 0.0f) {
        pos = target->header.m_vPos;
        func_0020EF2C(&d, &obj->header.m_vPos, &pos);
        heading = func_0020EAA0(&d);
        if (func_002685F0(target)) {
            push = 716.8f;
        } else {
            push = 179.2f;
        }
        target->m_vVelocity.x += push * func_002982F0(heading);
        target->m_vVelocity.z += push * func_002974C0(heading);
        if (D_8012F9E4.timer > 0.0f) {
            D_8012F9E4.timer -= D_800B6D28;
        }
    }
    if (ai->m_AnimRepeatedCount != 0 && D_8012F9E4.timer <= 0.0f && D_8012F9E4.pushTimer <= 0.0f) {
        D_8012F9E4.index = -1;
        obj->m_AI.m_AnimSpeed = 1.0f;
        func_00243414(obj, ai, 7);
    }
}
