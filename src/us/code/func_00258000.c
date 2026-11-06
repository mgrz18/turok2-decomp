#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"
#include "structs/weapon.h"
#include "globals/player.h"


extern u32 gFrameCountGameplay;
extern s32 gScene;
extern u8 gEngineApp_m_Game_m_CameraPool[];
extern struct {
    s32 enabled;
} gEngineApp_m_Settings_match_tag;
extern s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);
extern s32 CPlayer__GetNextUsableWeapon(CPlayer *player);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern void Sound__Play(s32 sound, CVector3 pos, CVector3 *source, s32 arg);
extern void CCameraPool__AddText(void *buf, s32 camera, s32 model);

/* Switches the weapon model to the player's next weapon (see WeaponActor_EquipRequested)
 * and puts the weapon AI back in mode 0. */
static __inline__ void switchWeapon(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player = (CPlayer *) obj->m_AI.m_pParent;

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || gScene != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        ai->m_pModeTable = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->modeTable;
        ai->m_pfEffects = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->effects;
        if (gFrameCountGameplay >= 5) {
            CCameraPool__AddText(gEngineApp_m_Game_m_CameraPool, player->m_pCamera, gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->name);
        }
        ai->m_Weapon.m_BarrelAngle = 0;
        ai->m_Weapon.m_BarrelSpeed = 0;
        ai->m_Weapon.m_nLod = 0;
        ai->m_Weapon.m_FireDelay = 0;
    }
    CAI__SetMode(obj, ai, 0);
}

/* First-person weapon, idle: picks another weapon when the current one runs
 * out, plays sound 0x27D8 on fire in tag mode, otherwise switches to the
 * queued weapon unless the pose (modes 1 and 7) or visibility forbids it. */
void WeaponMode_Idle_Update(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player;
    s32 mode;

    player = (CPlayer *) obj->m_AI.m_pParent;
    mode = gPlayerStateTable[player->m_Mode].pose;
    if (ai->m_AnimRepeatedCount && !CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon)) {
        player->m_RequestedWeapon = CPlayer__GetNextUsableWeapon(player);
    }
    player->m_ContState.m_pContState->rumbleBase = 0;
    ai->m_Weapon.m_SpinTime = 0;
    if (gEngineApp_m_Settings_match_tag.enabled && player->m_pOptions->m_TagMode) {
        if (player->m_ContState.m_dwButtonsSingle & 0x2000) {
            Sound__Play(0x27D8, player->m_CharacterActor.header.m_vPos, &player->m_CharacterActor.header.m_vPos, -1);
        }
        return;
    }
    if (mode == 1 || mode == 7) {
        return;
    }
    if (!ai->m_AnimRepeatedCount) {
        return;
    }
    switchWeapon(obj, ai);
}
