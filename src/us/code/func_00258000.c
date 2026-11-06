#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"
#include "structs/weapon.h"


typedef struct {
    /* 0x00 */ s16 mode;
    /* 0x02 */ u8 pad2[0x16];
} PoseInfo;   /* 0x18 bytes */

extern PoseInfo D_800ADF68[];
extern u32 D_800B6D20;
extern s32 D_800F7078;
extern u8 D_801198B0[];
extern struct {
    s32 enabled;
} D_8011ACE4;
extern s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);
extern s32 CPlayer__GetNextUsableWeapon(CPlayer *player);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern void Sound__Play(s32 sound, CVector3 pos, CVector3 *source, s32 arg);
extern void CCameraPool__AddText(void *buf, s32 camera, s32 model);

/* Switches the weapon model to the player's next weapon (see WeaponActor_EquipRequested)
 * and puts the weapon AI back in mode 0. */
static __inline__ void switchWeapon(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player = (CPlayer *) obj->m_AI.m_pParent;

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || D_800F7078 != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        ai->m_pModeTable = D_800AF618[player->m_Data.m_CurrentWeapon]->modeTable;
        ai->m_pfEffects = D_800AF618[player->m_Data.m_CurrentWeapon]->effects;
        if (D_800B6D20 >= 5) {
            CCameraPool__AddText(D_801198B0, player->m_pCamera, D_800AF618[player->m_Data.m_CurrentWeapon]->name);
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
    mode = D_800ADF68[player->m_Mode].mode;
    if (ai->m_AnimRepeatedCount && !CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon)) {
        player->m_RequestedWeapon = CPlayer__GetNextUsableWeapon(player);
    }
    player->m_ContState.m_pContState->rumbleBase = 0;
    ai->m_Weapon.m_SpinTime = 0;
    if (D_8011ACE4.enabled && player->m_pOptions->m_TagMode) {
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
