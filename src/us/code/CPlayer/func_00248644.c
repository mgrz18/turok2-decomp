#include "common.h"
#include "structs/weapon.h"
#include "globals/game.h"
#include "globals/scene.h"

s32 Level_GetHub(s32 arg0);

#ifdef TARGET_PORT
#include "structs/player.h"
/* Host body (round 7 v2): the raw N64 offsets of CPlayer by member (0x52C + n
 * m_Data.m_SpecialPickupCount[n], 0x9FC m_Data.m_NukePartLevelFlags, 0x988 m_WeaponList[22],
 * 0x934 m_AmmoList, 0xB14 m_RequestedWeapon). */
void CPlayer__CollectKeyPiece(u8 *arg0, s32 id) {
    CPlayer *player = (CPlayer *) arg0;
    u8 *key;
    u32 level;
    AmmoInfo *info;
    s16 *ammo;
    s32 count;
    s32 max;

    if (!(gEngineApp_m_Settings.cheatFlags & 0x10)) {
        key = &player->m_Data.m_SpecialPickupCount[id - 4000];
        key[0] += 0x11;
        if (id == 4500) {
            level = Level_GetHub(gScene_m_LevelID) - 1;
            if (level < 6) {
                player->m_Data.m_NukePartLevelFlags[level]++;
            }
            if ((s8) (key[0] & 0xF) >= 6) {
                player->m_Data.m_WeaponList[22] = 1;
                info = gWeaponInfoTable[22]->ammo[0];
                ammo = &player->m_Data.m_AmmoList[info->type];
                count = *ammo + 2;
                max = info->max;
                if (count < max) {
                    max = count;
                }
                *ammo = max;
                player->m_RequestedWeapon = 0x16;
            }
        }
    }
}
#else
/* Collects key piece (id - 4000); key 4500 also counts per level and, once the
   sixth piece is found, flags the player and gives two rounds of ammo. */
void CPlayer__CollectKeyPiece(u8 *player, s32 id) {
    u8 *key;
    u32 level;
    AmmoInfo *info;
    s16 *ammo;
    s32 count;
    s32 max;

    if (!(gEngineApp_m_Settings.cheatFlags & 0x10)) {
        key = player + (id - 4000);
        key[0x52C] += 0x11;
        if (id == 4500) {
            level = Level_GetHub(gScene_m_LevelID) - 1;
            if (level < 6) {
                (player + level)[0x9FC]++;
            }
            if ((s8) (key[0x52C] & 0xF) >= 6) {
                player[0x988] = 1;
                info = gWeaponInfoTable[22]->ammo[0];
                ammo = (s16 *) (player + (info->type * 2 + 0x934));
                count = *ammo + 2;
                max = info->max;
                if (count < max) {
                    max = count;
                }
                *ammo = max;
                *(s16 *) (player + 0xB14) = 0x16;
            }
        }
    }
}
#endif
