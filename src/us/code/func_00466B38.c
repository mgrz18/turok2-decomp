/* Second VM module copy of CPlayer__NewGame (same source, linked 0x0042DB38). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/camera.h"


extern s32 Demo_GetPlayerCount(void);
extern s32 SavedPlayers_IsActive(void);
extern u8 *CCameraPool__AllocCamera_2(void *);
extern void CBarrel__Reset(void *strokes);
extern void CPlayer__GiveStartingWeapons(CPlayer *p);
extern void CPlayer__ResetHealth_2(CPlayer *p);
extern void MenuPool_OpenMenu_2(void *, void *callback, CPlayer *p, s32 controller, s32 index);
extern void func_00438800();
extern void func_00438728();
extern void func_0043908C();

void CPlayer__NewGame_2(CPlayer *p, s32 character, s32 arg2) {
    GameSettings *flags;
    s32 i;

    if (Demo_GetPlayerCount()) {
        arg2 = 0;
    }
    if (SavedPlayers_IsActive()) {
        arg2 = 0;
    }
    p->m_bActive = 1;
    p->m_Data.m_nDeathmatchCharacter = character;
    p->m_pCamera = (struct CCamera *) CCameraPool__AllocCamera_2(&gEngineApp_m_Game_m_CameraPool);
    CBarrel__Reset(&p->m_Barrel);
    for (i = 0; i < 0x400; i++) {
        p->m_Data.m_SpecialPickupCount[i] = 0;
    }
    p->m_Data.m_LifeForce = 0;
    for (i = 0; i < 6; i++) {
        p->m_Data.m_NukePartLevelFlags[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        p->m_Data.m_KeySet6LevelFlags[i] = 0;
    }
    if (!gEngineApp_m_Settings.multiplayer) {
        p->m_Data.m_Lives = 3;
    } else {
        p->m_Data.m_Lives = 1;
    }
    CPlayer__GiveStartingWeapons(p);
    p->m_InvincibilityTime = 0;
    p->m_PlaneWalkerTime = 0;
    p->m_CharAnim = -1;
    p->m_LastCharAnim = -1;
    for (i = 0; i < 4; i++) {
        p->m_pOptions->m_KilledBy[i] = 0;
        p->m_pOptions->m_Killed[i] = 0;
        p->m_pOptions->m_TaggedBy[i] = 0;
        p->m_pOptions->m_Tagged[i] = 0;
    }
    p->m_pOptions->m_Suicides = 0;
    p->m_pOptions->m_Deaths = 0;
    p->m_pOptions->m_Kills = 0;
    p->m_pOptions->m_Tags = 0;
    p->m_pOptions->m_bDead = 0;
    p->m_pOptions->unk_0x3D = 0;
    p->m_pOptions->m_TagMode = 0;
    p->m_pOptions->m_LastTagMode = 0;
    for (i = 0; i < 6; i++) {
        p->m_Data.m_AdonAmmoFlagHelped[i] = 0;
        p->m_Data.m_AdonHealthFlagHelped[i] = 0;
    }
    p->m_Data.m_bLevel1TotemFlags = 0;
    p->m_Data.m_bLevel2TotemFlags = 0;
    p->m_Data.m_bLevel3TotemFlags = 0;
    p->m_Data.m_bLevel4TotemFlags = 0;
    p->m_Data.m_bLevel5TotemFlags = 0;
    p->m_Data.m_bBlindBossComplete = 0;
    p->m_Data.m_bQueenBossComplete = 0;
    p->m_Data.m_bMotherBossComplete = 0;
    p->m_Data.m_bPrimagenBossComplete = 0;
    p->m_Data.m_LevelIntroPlayedFlags = 0;
    p->m_Data.m_dwLevelFirstTimeFlags |= 1;
    CPlayer__ResetHealth_2(p);
    flags = &gEngineApp_m_Settings;
    if (flags->multiplayer) {
        p->m_nDeathmatchString = 0;
    }
    if (arg2) {
        if (flags->multiplayer) {
            p->m_pOptions->m_nCharacter = 0;
            p->m_Data.m_nDeathmatchCharacter = 0;
            p->m_bEnteringDeathmatch = 1;
            if (flags->match.teams) {
                MenuPool_OpenMenu_2(p->m_pCamera->m_MenuPool, func_00438800, p, (sptr) p->m_ContState.m_pContState, p->m_nController);
            } else {
                MenuPool_OpenMenu_2(p->m_pCamera->m_MenuPool, func_00438728, p, (sptr) p->m_ContState.m_pContState, p->m_nController);
            }
        } else {
            MenuPool_OpenMenu_2(p->m_pCamera->m_MenuPool, func_0043908C, p, (sptr) p->m_ContState.m_pContState, p->m_nController);
        }
    }
}
