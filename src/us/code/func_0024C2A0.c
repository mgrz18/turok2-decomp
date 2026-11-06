/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_6.CPP, func_802181FC_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/game.h"

extern s32 sBarrelLayoutGroup1[8];   /* layouts: group 1, group 0, flag 0x80; then the same for gEngineApp_m_Settings_multiplayer */
extern s32 sBarrelLayoutGroup0[8];
extern s32 sBarrelLayoutUnderwater[8];
extern s32 sBarrelLayoutGroup1MP[8];
extern s32 sBarrelLayoutGroup0MP[8];
extern s32 sBarrelLayoutUnderwaterMP[8];
extern f32 gBarrelSlotScale;              /* slot scale */

s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);

/* Lays out the eight-slot weapon wheel from the layout table for :
 * each weapon becomes its alternate when the player owns that (and is not
 * holding it), the held weapon gets the cursor, and the slots are set
 * 45 degrees apart. */
void CBarrel__LayoutEight(CBarrel *menu, s32 group, CPlayer *player) {
    s32 *layout;
    s32 i;
    s32 w;
    s16 alt;
    s32 ok;

    if (group != 0) {
        layout = sBarrelLayoutGroup1;
        menu->m_bUnderwater = 0;
        if (gEngineApp_m_Settings_multiplayer != 0) {
            layout = sBarrelLayoutGroup1MP;
        }
    } else {
        layout = sBarrelLayoutGroup0;
        menu->m_bUnderwater = 0;
        if (gEngineApp_m_Settings_multiplayer != 0) {
            layout = sBarrelLayoutGroup0MP;
        }
    }
    if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
        layout = sBarrelLayoutUnderwater;
        if (gEngineApp_m_Settings_multiplayer != 0) {
            layout = sBarrelLayoutUnderwaterMP;
        }
        menu->m_bUnderwater = 1;
    }
    menu->m_Group = group;
    menu->m_nCursor = -1;
    for (i = 0; i < 8; i++) {
        w = layout[i];
        if (w == -1) {
            menu->m_Items[i].m_Weapon = w;
            menu->m_Items[i].m_bAvailable = 0;
        } else {
            if (!(player->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
                alt = gWeaponInfoTable[w]->alternate;
                if (alt != -1 && player->m_Data.m_WeaponList[alt] != 0 && player->m_Data.m_CurrentWeapon != alt) {
                    w = alt;
                }
            }
            menu->m_Items[i].m_Weapon = w;
            if (player->m_Data.m_CurrentWeapon == w) {
                menu->m_nCursor = i;
            }
            menu->m_Items[i].m_pInfo = gWeaponInfoTable[w];
            if (CPlayer__GetWeaponAmmo(player, w) == 0 || player->m_Data.m_WeaponList[w] == 0) {
                ok = 0;
            } else {
                ok = 1;
            }
            menu->m_Items[i].m_bAvailable = ok;
            menu->m_Items[i].m_Lift = 0;
            menu->m_Items[i].m_Scale = gBarrelSlotScale;
            menu->m_Items[i].m_Angle = i * 0.785398245f;
        }
    }
    menu->m_bStickWheel = 1;
}
