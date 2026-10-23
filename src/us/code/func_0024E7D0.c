#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s16 *ammo[3]; /* each points at an ammo type id; [1] is the lower ammo */
} WeaponDef;

extern WeaponDef *D_800AF618[];

/* func_0025398C, inlined. */
static __inline__ s16 *GetWeaponAmmo(CPlayer *player, s32 weapon) {
    s16 *ammo;
    s16 *found;
    s32 i;
    WeaponDef *def = D_800AF618[weapon];

    if (player->m_UseLowerAmmo && (ammo = def->ammo[1]) != NULL
        && player->m_Data.m_AmmoList[*ammo] > 0) {
        return ammo;
    }
    found = NULL;
    for (i = 0; i < 3; i++) {
        ammo = def->ammo[i];
        if (ammo == NULL) {
            break;
        }
        found = ammo;
        if (player->m_Data.m_AmmoList[*ammo] > 0) {
            return found;
        }
    }
    return found;
}

extern s16 D_800AF6A8[];

/* D_800AF6A8 entry of the current weapon ammo type, -1 if none. */
s32 func_0024E7D0(CPlayer *player) {
    s16 *ammo;
    s32 type;
    s32 value;

    ammo = GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon);
    type = (ammo != NULL) ? *ammo : -1;
    value = -1;
    if (type != -1) {
        value = D_800AF6A8[type];
    }
    return value;
}
