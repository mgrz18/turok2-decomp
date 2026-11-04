/* Cut fixed in versions/cuts.us.txt: func_00253A1C really is 0x253A1C-0x253AC0 (0xA4 bytes): splat split off its tail (return *ammo) as func_00253AB4. */
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

/* Ammo type a weapon fires now, -1 if it has none. */
s32 func_00253A1C(CPlayer *player, s32 weapon) {
    AmmoInfo *ammo = GetWeaponAmmo(player, weapon);

    if (ammo != NULL) {
        return ammo->type;
    }
    return -1;
}
