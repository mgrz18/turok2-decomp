#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

extern s32 D_8011AAF1; /* a u8 game-mode flag; D_8011AAA0 is addressed through it */

#define D_8011AAA0 (*(s32 *) ((u8 *) &D_8011AAF1 - 0x51))

void func_002573FC(CPlayer *arg0) {
    s16 temp_a1;
    WeaponInfo *temp_v0;

    if ((*(u8 *) &D_8011AAF1 == 0) && ((arg0->m_Barrel.m_bUnderwater == 0) || (arg0->m_Barrel.m_nItems == 0)) && ((D_8011AAA0 == 0) || !(arg0->m_ContState.m_dwButtons & 0x20)) && (arg0->m_ContState.m_dwButtonsSingle & 0x200)) {
        temp_v0 = D_800AF618[arg0->m_RequestedWeapon];
        temp_a1 = temp_v0->alternate;
        if ((temp_a1 != -1) && (arg0->m_Data.m_WeaponList[temp_a1] != 0)) {
            arg0->m_RequestedWeapon = (s16) (u16) temp_v0->alternate;
        }
    }
}
