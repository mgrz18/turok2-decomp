#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Picks one of the four spawn points at random, at least 460.8 away from the module object's target (when
 * it has one), and stores it in out. */


extern f32 func_0040BD60(void *obj, CGameObjectInstance **found);
extern f32 func_002119FC(f32 min, f32 max);
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */

void func_0040C19C(CVector3 *out) {
    CGameObjectInstance *found;
    s32 ok;
    s32 i;

    func_0040BD60(D_8012FB70.obj, &found);
    do {
        ok = 1;
        i = func_002119FC(0.0f, 0.99999f) * 4.0f;
        if (found != NULL) {
            func_0020EF2C(out, &found->header.m_vPos, &D_00431D20[i].pos);
            if (func_00298470(out->x * out->x + out->y * out->y + out->z * out->z) < 460.8f) {
                ok = 0;
            }
        }
    } while (!ok);
    *out = D_00431D20[i].pos;
}
