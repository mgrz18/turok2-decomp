#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

s32 CScene__SetInstanceObjectType(s32 *, void *, u16, s32); /* extern */

extern s32 gScene;

void CPlayer__LoadWeaponModel(CPlayer *arg0) {
    CScene__SetInstanceObjectType(&gScene, &arg0->m_WeaponActor, gWeaponInfoTable[arg0->m_Data.m_CurrentWeapon]->model, arg0->m_WeaponActor.m_AI.m_pModeTable->unk_10);
}
