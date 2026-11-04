#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

extern s32 func_0024E700(CPlayer *player, s32 weapon);

/* Fills entry index with a weapon, selecting it if none is and it is the
 * player's requested weapon. */
void func_002550E8(CBarrel *list, s32 weapon, s32 index, CPlayer *player) {
    WeaponInfo *def = D_800AF618[weapon];
    s32 hasAmmo = func_0024E700(player, weapon) != 0;

    list->m_Items[index].m_Weapon = weapon;
    list->m_Items[index].m_bAvailable = hasAmmo;
    list->m_Items[index].m_pInfo = def;
    if (list->m_nCursor == -1 && weapon == player->m_RequestedWeapon) {
        list->m_nCursor = index;
        list->m_Spin = list->m_Items[index].m_Angle;
    }
    list->m_nCount++;
}
