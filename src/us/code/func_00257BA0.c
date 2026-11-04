#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

extern s32 D_80110058;             /* first weapon - 1 */
extern s32 D_8011005C;             /* weapon count */
extern u8 D_8011AAF1;
extern s32 func_0024E700(CPlayer *player, s32 weapon);

/* The next weapon the player can switch to: the first usable one after the
 * current, else the last usable one before it (weapons 12 and 23 are never
 * picked going up, 12 never going down); 0x18 or 0 when there is none. */
s32 func_00257BA0(CPlayer *player) {
    s32 i;
    s32 usable;

    if (player->m_Data.m_CurrentWeapon < D_8011005C) {
        for (i = player->m_Data.m_CurrentWeapon + 1; i < D_8011005C; i++) {
            if (func_0024E700(player, i) && player->m_Data.m_WeaponList[i] && i != 23 && i != 12) {
                if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
                    usable = D_800AF618[i]->flags & 2;
                } else {
                    usable = D_800AF618[i]->flags & 1;
                }
                if (usable) {
                    return i;
                }
            }
        }
    }
    for (i = player->m_Data.m_CurrentWeapon - 1; D_80110058 < i; i--) {
        if (func_0024E700(player, i) && player->m_Data.m_WeaponList[i] && i != 12) {
            if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
                usable = D_800AF618[i]->flags & 2;
            } else {
                usable = D_800AF618[i]->flags & 1;
            }
            if (usable) {
                return i;
            }
        }
    }
    if (D_8011AAF1) {
        return 0x18;
    }
    return 0;
}
