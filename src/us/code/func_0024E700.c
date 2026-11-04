#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

/* func_0025398C, inlined. */
static __inline__ AmmoInfo *GetWeaponAmmo(CPlayer *player, s32 weapon) {
    AmmoInfo *ammo;
    AmmoInfo *found;
    s32 i;
    WeaponInfo *def = D_800AF618[weapon];

    if (player->m_UseLowerAmmo && (ammo = def->ammo[1]) != NULL
        && player->m_Data.m_AmmoList[ammo->type] > 0) {
        return ammo;
    }
    found = NULL;
    for (i = 0; i < 3; i++) {
        ammo = def->ammo[i];
        if (ammo == NULL) {
            break;
        }
        found = ammo;
        if (player->m_Data.m_AmmoList[ammo->type] > 0) {
            return found;
        }
    }
    return found;
}

/* How many rounds of its ammo a weapon has, -1 if none. */
s32 func_0024E700(CPlayer *player, s32 weapon) {
    AmmoInfo *ammo;
    s32 type;
    s32 count;

    if (weapon == 0x15 && (s8)player->m_Data.m_WeaponList[0x15] != 0) {
        return -1;
    }
    ammo = GetWeaponAmmo(player, weapon);
    type = (ammo != NULL) ? ammo->type : -1;
    count = -1;
    if (type != -1) {
        count = player->m_Data.m_AmmoList[type];
    }
    return count;
}
