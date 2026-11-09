/* Player code: CPlayer__NewGame, 0x0042DC18-0x0042DE74 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "protos/menu.h"


extern s32 Demo_GetPlayerCount(void);
extern s32 SavedPlayers_IsActive(void);
extern u8 *CCameraPool__AllocCamera(void *);
extern void CBarrel__Reset(void *strokes);
extern void CPlayer__GiveStartingWeapons(CPlayer *p);
extern void CPlayer__ResetHealth(CPlayer *p);
extern void func_004388E0();
extern void func_00438808();
extern void func_0043916C();

void CPlayer__NewGame(CPlayer *p, s32 character, s32 arg2) {
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
    p->m_pCamera = (struct CCamera *) CCameraPool__AllocCamera(&gEngineApp_m_Game_m_CameraPool);
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
    CPlayer__ResetHealth(p);
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
                MenuPool_OpenMenu(p->m_pCamera->m_MenuPool, func_004388E0, p, (sptr) p->m_ContState.m_pContState, p->m_nController);
            } else {
                MenuPool_OpenMenu(p->m_pCamera->m_MenuPool, func_00438808, p, (sptr) p->m_ContState.m_pContState, p->m_nController);
            }
        } else {
            MenuPool_OpenMenu(p->m_pCamera->m_MenuPool, func_0043916C, p, (sptr) p->m_ContState.m_pContState, p->m_nController);
        }
    }
}
