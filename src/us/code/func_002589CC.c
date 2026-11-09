#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/camera.h"
#include "globals/scene.h"
#include "globals/engine.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

extern void CCameraPool__AddText(void *arg0, void *camera, s32 model);

/* Same body as WeaponActor_SwitchToRequested. Switches the weapon actor to its owner's requested weapon. */
void WeaponMode_Switch_Enter(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)weapon)->m_AI.m_pParent);
    s16 requested = player->m_RequestedWeapon;

    if (player->m_Data.m_CurrentWeapon != requested || gScene.m_Mode != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        state->m_pModeTable = gWeaponInfoTable[requested]->modeTable;
        state->m_pfEffects = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->effects;
        if (gFrameCountGameplay >= 5) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->name);
        }
        state->m_Weapon.m_BarrelAngle = 0;
        state->m_Weapon.m_BarrelSpeed = 0;
        state->m_Weapon.m_nLod = 0;
        state->m_Weapon.m_FireDelay = 0;
    }
}
