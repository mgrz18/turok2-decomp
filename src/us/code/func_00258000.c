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
extern s32 func_0024E700(CPlayer *player, s32 weapon);
extern s32 func_00257BA0(CPlayer *player);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern void func_00275544(s32 sound, CVector3 pos, CVector3 *source, s32 arg);
extern void func_0027FC2C(void *buf, s32 camera, s32 model);

/* Switches the weapon model to the player's next weapon (see func_002596B4)
 * and puts the weapon AI back in mode 0. */
static __inline__ void switchWeapon(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player = (CPlayer *) obj->m_AI.m_pParent;

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || D_800F7078 != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        ai->m_pModeTable = D_800AF618[player->m_Data.m_CurrentWeapon]->modeTable;
        ai->m_pfEffects = D_800AF618[player->m_Data.m_CurrentWeapon]->effects;
        if (D_800B6D20 >= 5) {
            func_0027FC2C(D_801198B0, player->m_pCamera, D_800AF618[player->m_Data.m_CurrentWeapon]->name);
        }
        ai->m_Weapon.m_BarrelAngle = 0;
        ai->m_Weapon.m_BarrelSpeed = 0;
        ai->m_Weapon.m_nLod = 0;
        ai->m_Weapon.m_FireDelay = 0;
    }
    func_00243414(obj, ai, 0);
}

/* First-person weapon, idle: picks another weapon when the current one runs
 * out, plays sound 0x27D8 on fire in tag mode, otherwise switches to the
 * queued weapon unless the pose (modes 1 and 7) or visibility forbids it. */
void func_00258000(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player;
    s32 mode;

    player = (CPlayer *) obj->m_AI.m_pParent;
    mode = D_800ADF68[player->m_Mode].mode;
    if (ai->m_AnimRepeatedCount && !func_0024E700(player, player->m_Data.m_CurrentWeapon)) {
        player->m_RequestedWeapon = func_00257BA0(player);
    }
    player->m_ContState.m_pContState->rumbleBase = 0;
    ai->m_Weapon.m_SpinTime = 0;
    if (D_8011ACE4.enabled && player->m_pOptions->m_TagMode) {
        if (player->m_ContState.m_dwButtonsSingle & 0x2000) {
            func_00275544(0x27D8, player->m_CharacterActor.header.m_vPos, &player->m_CharacterActor.header.m_vPos, -1);
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
