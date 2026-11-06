#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

extern f32 D_800AE2D8;              /* slot scale */

s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);

/* Fills the weapon menu with the weapons flagged in `owned` (35 entries,
 * `rows` times over), spreading the slots evenly around the wheel. */
void CBarrel__Fill(CBarrel *menu, s32 rows, s32 *owned, s32 perRow, CPlayer *player) {
    s32 i;
    s32 n;
    s32 w;
    WeaponInfo *info;
    s32 ok;

    menu->m_nCount = 0;
    for (i = 0; i < perRow * rows; i++) {
        menu->m_Items[i].m_Angle = -(i * (360.0f / (perRow * rows)) * 0.017453294f);
        menu->m_Items[i].m_Lift = 0;
        menu->m_Items[i].m_Scale = D_800AE2D8;
    }
    n = 0;
    for (i = 0; i < rows; i++) {
        for (w = 0; w < 35; w++) {
            if (owned[w] != 0) {
                info = D_800AF618[w];
                ok = CPlayer__GetWeaponAmmo(player, w) != 0;
                menu->m_Items[n].m_Weapon = w;
                menu->m_Items[n].m_bAvailable = ok;
                menu->m_Items[n].m_pInfo = info;
                if (menu->m_nCursor == -1 && w == player->m_RequestedWeapon) {
                    menu->m_nCursor = n;
                    menu->m_Spin = menu->m_Items[n].m_Angle;
                }
                n++;
                menu->m_nCount++;
            }
        }
    }
}
