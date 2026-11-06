#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/game.h"

extern s32 gScene_m_PreviewState;             /* first weapon - 1 */
extern s32 gScene_m_PreviewTimer;             /* weapon count */
extern s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);

/* The next weapon the player can switch to: the first usable one after the
 * current, else the last usable one before it (weapons 12 and 23 are never
 * picked going up, 12 never going down); 0x18 or 0 when there is none. */
s32 CPlayer__GetNextUsableWeapon(CPlayer *player) {
    s32 i;
    s32 usable;

    if (player->m_Data.m_CurrentWeapon < gScene_m_PreviewTimer) {
        for (i = player->m_Data.m_CurrentWeapon + 1; i < gScene_m_PreviewTimer; i++) {
            if (CPlayer__GetWeaponAmmo(player, i) && player->m_Data.m_WeaponList[i] && i != 23 && i != 12) {
                if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
                    usable = gWeaponInfoTable[i]->flags & 2;
                } else {
                    usable = gWeaponInfoTable[i]->flags & 1;
                }
                if (usable) {
                    return i;
                }
            }
        }
    }
    for (i = player->m_Data.m_CurrentWeapon - 1; gScene_m_PreviewState < i; i--) {
        if (CPlayer__GetWeaponAmmo(player, i) && player->m_Data.m_WeaponList[i] && i != 12) {
            if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
                usable = gWeaponInfoTable[i]->flags & 2;
            } else {
                usable = gWeaponInfoTable[i]->flags & 1;
            }
            if (usable) {
                return i;
            }
        }
    }
    if (gEngineApp_m_Settings.multiplayer) {
        return 0x18;
    }
    return 0;
}
