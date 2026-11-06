/* Second-module twin of MotherBoss_RandomSpawnPoint (VM module 2 is linked over module 1; same source, its own strings and pool). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Picks one of the four spawn points at random, at least 460.8 away from the module object's target (when
 * it has one), and stores it in out. */


extern f32 MotherBoss_GetTargetDist(void *obj, CGameObjectInstance **found);
extern f32 Random_Blend(f32 min, f32 max);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */

void MotherBoss_RandomSpawnPoint_2(CVector3 *out) {
    CGameObjectInstance *found;
    s32 ok;
    s32 i;

    MotherBoss_GetTargetDist(D_8012FB70.obj, &found);
    do {
        ok = 1;
        i = Random_Blend(0.0f, 0.99999f) * 4.0f;
        if (found != NULL) {
            CVector3__Subtract(out, &found->header.m_vPos, &D_00431C40[i].pos);
            if (func_00298470(out->x * out->x + out->y * out->y + out->z * out->z) < 460.8f) {
                ok = 0;
            }
        }
    } while (!ok);
    *out = D_00431C40[i].pos;
}
