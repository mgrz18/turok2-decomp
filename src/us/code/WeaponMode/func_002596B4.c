#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/camera.h"
#include "globals/scene.h"
#include "globals/engine.h"

/* Host body: the N64 State view reads the CAI at N64 offsets (mode table 0x2C, effects 0x108,
 * the weapon vars at 0x10C..); on the host they are members (m_Weapon: CWeaponAIVars). */
void CCameraPool__AddText(void *buf, sptr a, sptr b);
void CPlayer__StartWeaponBackgroundSound(CPlayer *player, s32 id);

void WeaponActor_EquipRequested(CGameObjectInstance *actor, CAI *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || gScene.m_Mode != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        state->m_pModeTable = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->modeTable;
        state->m_pfEffects = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->effects;
        if (gFrameCountGameplay >= 5) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, (sptr) player->m_pCamera,
                                 (sptr) gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->name);
        }
        state->m_Weapon.m_BarrelAngle = 0;
        state->m_Weapon.m_BarrelSpeed = 0;
        state->m_Weapon.m_nLod = 0;
        state->m_Weapon.m_FireDelay = 0;
    }
    CPlayer__StartWeaponBackgroundSound(player, 0x1A8);
}
