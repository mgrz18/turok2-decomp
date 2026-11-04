#include "common.h"
#include "structs/game_object.h"

/* AI state: turn toward the point (0, 0, 512) and go to anim state 0x1C once within 51.2 of it. */

extern f32 func_00246310(CGameObjectInstance *obj, CVector3 target);
extern void func_00245BAC(CGameObjectInstance *obj, void *ai, f32 angle, f32 turn);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern void func_00243414(CGameObjectInstance *obj, void *ai, s32 state);

void func_0040B918(CGameObjectInstance *obj, void *ai) {
    CVector3 d;

    d.x = 0.0f;
    d.y = 0.0f;
    d.z = 512.0f;
    func_00245BAC(obj, ai, 0.261799425f, func_00246310(obj, d));
    func_0020EF2C(&d, &d, &obj->header.m_vPos);
    if (func_00298470(d.x * d.x + d.y * d.y + d.z * d.z) < 51.199997f) {
        func_00243414(obj, ai, 0x1C);
    }
}
