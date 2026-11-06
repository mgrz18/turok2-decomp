/* Cut fixed in versions/cuts.us.txt: CPlayer__GetMaxAmmo really is 0x253AC0-0x253B54 (0x94 bytes, with the trailing nop): splat split it at its loop head as func_00253AD4. */
#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

/* Most rounds of an ammo type any weapon carries (only held weapons if heldOnly). */
s32 CPlayer__GetMaxAmmo(CPlayer *player, s32 type, s32 heldOnly) {
    s32 max;
    s32 w;
    s32 i;
    WeaponInfo *def;
    AmmoInfo *ammo;
    s32 n;

    if (type == -1) {
        return 0;
    }
    max = 0;
    for (w = 0; w < 35; w++) {
        if (heldOnly && (s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        def = D_800AF618[w];
        for (i = 0; i < 3; i++) {
            ammo = def->ammo[i];
            if (ammo == NULL) {
                break;
            }
            if (ammo->type == type) {
                n = ammo->max;
                if (n < max) {
                    n = max;
                }
                max = n;
            }
        }
    }
    return max;
}
