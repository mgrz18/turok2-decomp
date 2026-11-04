#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

/* Fills arg1[0..34] with whether each weapon can be selected and returns how many can.
 * The m_WeaponList[i] reads are written as (player + i)->m_WeaponList[0], read signed: the code adds the
 * index to the player first. */
s32 func_00255048(CPlayer *arg0, s32 *arg1) {
    s16 temp_a2;
    s32 *var_a1;
    s32 var_a3;
    s32 var_t0;
    s32 var_t1;
    s32 var_v0;
    s32 var_v1;
    WeaponInfo *temp_v1;

    var_a1 = arg1;
    var_t1 = 0;
    var_a3 = 0;
    var_t0 = 0;
    do {
        temp_v1 = *(WeaponInfo **) ((s8 *) D_800AF618 + var_t0);
        if (*(s8 *) ((CPlayer *) ((s8 *) arg0 + var_a3))->m_Data.m_WeaponList == 0) {
            var_v1 = 0;
        } else {
            if (arg0->m_WeaponActor.m_AI.m_dwFlags & 0x80) {
                var_v0 = temp_v1->flags & 2;
            } else {
                var_v0 = temp_v1->flags & 1;
            }
            if (var_v0 == 0) {
                var_v1 = 0;
            } else {
                temp_a2 = temp_v1->alternate;
                var_v1 = 1;
                if (*(s8 *) ((CPlayer *) ((s8 *) arg0 + temp_a2))->m_Data.m_WeaponList != 0) {
                    var_v1 = (var_a3 < temp_a2) ^ 1;
                }
            }
        }
        *var_a1 = var_v1;
        if (var_v1 != 0) {
            var_t1 += 1;
        }
        var_a1 = (__typeof__(var_a1))((s8 *)((__typeof__(var_a1))((s8 *)var_a1 + 4)));
        var_a3 += 1;
        var_t0 += 4;
    } while (var_a3 < 0x23);
    return var_t1;
}
