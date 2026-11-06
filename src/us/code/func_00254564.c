#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"
#include "structs/weapon.h"

s32 CPlayer__HasScope(CPlayer *arg0) {
    return (s16) gWeaponInfoTable[arg0->m_Data.m_CurrentWeapon]->zoomModel > 0;
}
