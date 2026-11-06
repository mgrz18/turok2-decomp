#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

/* CPlayer__GetFiringAmmo, inlined. */
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

extern s16 D_800AF6A8[];

/* D_800AF6A8 entry of the current weapon ammo type, -1 if none. */
s32 CPlayer__GetCurrentAmmoInfo(CPlayer *player) {
    AmmoInfo *ammo;
    s32 type;
    s32 value;

    ammo = GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon);
    type = (ammo != NULL) ? ammo->type : -1;
    value = -1;
    if (type != -1) {
        value = D_800AF6A8[type];
    }
    return value;
}
