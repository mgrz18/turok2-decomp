#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

/* Fills the ammo of every held weapon to its maximum; returns whether any
 * ammo was added. */
s32 CPlayer__FillAllAmmo(CPlayer *player) {
    s32 added = 0;
    s32 w;
    s32 i;
    WeaponInfo *def;
    AmmoInfo *ammo;
    s16 *count;
    s16 n;

    for (w = 0; w < 35; w++) {
        def = gWeaponInfoTable[w];
        if ((s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        for (i = 0; i < 3; i++) {
            ammo = def->ammo[i];
            if (ammo == NULL) {
                break;
            }
            count = &player->m_Data.m_AmmoList[ammo->type];
            if (*count < ammo->max) {
                added = 1;
            }
            n = ammo->max;
            if (ammo->max < *count) {
                n = *count;
            }
            *count = n;
        }
    }
    return added;
}
