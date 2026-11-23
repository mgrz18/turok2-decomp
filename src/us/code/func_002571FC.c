#include "common.h"

#include "structs/weapon.h"
/* Host body: the list is the CBarrel (m_nCount, m_Items[]: m_Weapon, m_bAvailable); the
 * N64 view puts the items at 0x1C with the N64 stride 0x18 (CBarrelItem holds a pointer). */
extern s32 CPlayer__GetWeaponAmmo(void *player, s32 id);

void CBarrel__MarkUsable(CBarrel *list, void *player) {
    s32 i;

    for (i = 0; i < list->m_nCount; i++) {
        if (CPlayer__GetWeaponAmmo(player, list->m_Items[i].m_Weapon)) {
            list->m_Items[i].m_bAvailable = 1;
        } else {
            list->m_Items[i].m_bAvailable = 0;
        }
    }
}
