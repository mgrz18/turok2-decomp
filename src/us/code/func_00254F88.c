#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

/* Whether the player holds a usable weapon that is not in the list. */
s32 CBarrel__HasUnlistedWeapon(CBarrel *list, CPlayer *player) {
    WeaponInfo *def;
    s32 usable;
    s32 found;
    s32 w;
    s32 i;
    s32 *aiFlags = &player->m_WeaponActor.m_AI.m_dwFlags;

    for (w = 0; w < 35; w++) {
        def = D_800AF618[w];
        usable = 0;
        if ((s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        if (*aiFlags & 0x80) {
            if (def->flags & 2) {
                usable = 1;
            }
        } else if (def->flags & 1) {
            usable = 1;
        }
        if (usable) {
            found = 0;
            for (i = 0; i < list->m_nCount; i++) {
                if (list->m_Items[i].m_Weapon == w) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                return 1;
            }
        }
    }
    return 0;
}
