/* Cut fixed in versions/cuts.us.txt: func_0025398C really is 0x25398C-0x253A1C (0x90 bytes): splat cut it at 0x2539DC (.L002539DC), the loop that follows is its body. */
#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s16 *ammo[3]; /* each points at an ammo type id; [1] is the lower ammo */
} WeaponDef;

extern WeaponDef *D_800AF618[];

/* The ammo entry a weapon fires now: the lower ammo when selected and stocked,
 * else the first stocked one, else the last one listed. */
s16 *func_0025398C(CPlayer *player, s32 weapon) {
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
