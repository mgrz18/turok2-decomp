/* Cut fixed in versions/cuts.us.txt: CPlayer__GetFiringAmmo really is 0x25398C-0x253A1C (0x90 bytes): splat cut it at 0x2539DC (.L002539DC), the loop that follows is its body. */
#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

/* The ammo entry a weapon fires now: the lower ammo when selected and stocked,
 * else the first stocked one, else the last one listed. */
AmmoInfo *CPlayer__GetFiringAmmo(CPlayer *player, s32 weapon) {
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
