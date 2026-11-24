/* Second VM module copy of CPlayer__SetupActors (same source, linked 0x0042D980). */
#include "common.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/player.h"
#include "globals/game.h"
#include "protos/ai.h"

extern void CGameObjectInstance__ConstructFromRecord(void *obj, void *rec, s32 mode, s32 index);
extern void CGameObjectInstance__CalculateOrientationMatrix();
extern void Actor_PlayRangedSound();
extern void Player_OnMultiplayerKill();
extern void CPlayer__OnParticleHitCallback();
extern void Player_DrawNodeCallback();
extern void WeaponActor_BuildMatrix();
extern void WeaponActor_SwitchToRequested(void *, void *);
extern void CPlayer__UpdateOrbitCamera(CPlayer *p, CPlayer *q);
extern void Cinema_UpdateCameraIfActive(sptr);

void CPlayer__SetupActors_2(CPlayer *p, void *rec, s32 mode) {
    f32 half;

    p->m_CharacterActor.m_AI.m_pParent = p;
    CGameObjectInstance__ConstructFromRecord(p, rec, mode, 0);
    p->m_CharacterActor.m_AI.m_pParent = p;
    p->m_CharacterActor.m_dwFlags |= 0x100000;
    p->m_CharacterActor.m_dwFlags |= 0x10000;
    p->m_CharacterActor.m_dwFlags |= 0x1000000;
    p->m_CharacterActor.m_dwFlags |= 0x2000000;
    if (!gEngineApp_m_Settings.multiplayer) {
        p->m_CharacterActor.m_dwFlags |= 0x10000000;
    }
    p->m_CharacterActor.m_AI.m_pfCalculateOrientationMatrix = CGameObjectInstance__CalculateOrientationMatrix;
    p->m_CharacterActor.m_AI.m_pfEffects = Actor_PlayRangedSound;
    p->m_CharacterActor.m_AI.m_pfHurt = Player_OnMultiplayerKill;
    p->m_CharacterActor.m_AI.m_pfHit = CPlayer__OnParticleHitCallback;
    if (gEngineApp_m_Settings.multiplayer) {
        p->m_CharacterActor.m_AI.m_pfDraw = Player_DrawNodeCallback;
    }

    p->m_WeaponActor.m_AI.m_pParent = p;
    CGameObjectInstance__ConstructFromRecord(&p->m_WeaponActor, rec, mode, 0);
    p->m_WeaponActor.m_AI.m_pfCalculateOrientationMatrix = WeaponActor_BuildMatrix;
    p->m_WeaponActor.m_AI.m_Mode = -1;
    p->m_WeaponActor.m_AI.m_pParent = p;
    p->m_WeaponActor.m_AI.m_pfHurt = Player_OnMultiplayerKill;
    p->m_WeaponActor.m_AI.m_pfHit = CPlayer__OnParticleHitCallback;
    p->m_WeaponActor.m_dwFlags |= 0x200000;
    p->m_Data.m_CurrentWeapon = -1;
    half = 0.5f;
    p->m_WeaponActor.m_vScale.x = half;
    p->m_WeaponActor.m_vScale.y = half;
    p->m_WeaponActor.m_vScale.z = half;
    p->m_WeaponActor.m_dwFlags &= ~0x10000;
    CAI__SetMode(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 0);
    WeaponActor_SwitchToRequested(&p->m_WeaponActor, &p->m_WeaponActor.m_AI);
    CPlayer__UpdateOrbitCamera(p, p);
    Cinema_UpdateCameraIfActive(p->m_pCamera);
    p->m_CharacterActor.m_vScale.x = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleZ;
}

#include "structs/camera.h"
#include "globals/camera.h"

extern s32 Demo_GetPlayerCount(void);
extern s32 SavedPlayers_IsActive(void);
extern u8 *CCameraPool__AllocCamera_2(void *);
extern void CBarrel__Reset(void *strokes);
extern void CPlayer__GiveStartingWeapons(CPlayer *p);
void CPlayer__ResetHealth_2(CPlayer *p);
extern void MenuPool_OpenMenu_2(void *, void *callback, CPlayer *p, sptr controller, sptr index);
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

#include "globals/scene.h"
#include "globals/render.h"
#include "globals/player.h"
#include "protos/game.h"

extern void CPlayer__PickSpawnPoint(void);
extern void CPlayer__LoadCharacterModel(CPlayer *p);
extern void COverlay__Construct(COverlay *item, s32 x, s32 y);
extern void COverlay__StartScript(COverlay *item, s32 *script);
extern void CPlayer__Reset(CPlayer *p);
extern s32 Cinema_IsActive(void);
extern s16 *CPlayer__GetFiringAmmo(CPlayer *p, s32);
extern void Cinema_Disable(void);
extern void Cinema_Enable(void);
extern void CGameRegion__FireTriggers(sptr, s32, CPlayer *p);

void CPlayer__LevelStart_2(CPlayer *p) {
    GameSettings *flags = &gEngineApp_m_Settings;
    CPlayerOverlay *c;
    s32 warp;

    if (flags->multiplayer) {
        CPlayer__PickSpawnPoint();
        CPlayer__LoadCharacterModel(p);
    }
    c = &p->m_Overlay;
    c->m_Mode = 0;
    c->m_rpObject = NULL;
    c->m_ModeTime = 0.0f;
    c->m_DesiredObjectType = -1;
    c->m_ObjectType = -1;
    c->m_vPos.x = 0.0f;
    c->m_vRot.x = 0.0f;
    c->m_vRot.z = 0.0f;
    c->m_vPos.y = 437.5f;
    c->m_vPos.z = -675.0f;
    c->m_vScale.x = 0.25f;
    c->m_vScale.y = 0.1875f;
    c->m_vScale.z = 0.25f;
    c->m_vRot.y = 1.5707965f;
    p->m_pRumble = NULL;
    if (!flags->multiplayer) {
        Controller_SetRumbleOption(p->m_ContState.m_pContState, 1);
    }
    p->m_FlashlightTime = 0.0f;
    p->m_pDLFlashlight1 = NULL;
    p->m_pDLFlashlight2 = NULL;
    p->m_pDLDarkCityLight = NULL;
    COverlay__Construct(&p->m_AmmoOverlay, 120, 96);
    COverlay__StartScript(&p->m_AmmoOverlay, gAmmoOverlayScript);
    COverlay__Construct(&p->m_HealthOverlay, 48, 96);
    COverlay__StartScript(&p->m_HealthOverlay, gHealthOverlayScript);
    COverlay__Construct(&p->m_LivesOverlay, -64, 16);
    COverlay__StartScript(&p->m_LivesOverlay, gLivesOverlayScript);
    COverlay__Construct(&p->m_TokensOverlay, 40, 16);
    COverlay__StartScript(&p->m_TokensOverlay, gTokensOverlayScript);
    COverlay__Construct(&p->m_TotalKillsOverlay, 40, 42);
    COverlay__StartScript(&p->m_TotalKillsOverlay, gTotalKillsOverlayScript);
    COverlay__Construct(&p->m_TotalTagsOverlay, 40, 68);
    COverlay__StartScript(&p->m_TotalTagsOverlay, gTotalTagsOverlayScript);
    COverlay__Construct(&p->m_AirOverlay, 24, (s16) (gScreenHeight + 96));
    warp = 0;
    COverlay__Construct(&p->m_BossOverlay, 24, (s16) (gScreenHeight + 96));
    p->m_SfxTimer = 0.0f;
    p->m_SightIconFrame = 0.0f;
    p->PreviousCameraQuatern = p->m_pCamera->m_qViewAlign;
    CPlayer__Reset(p);
    if (!Cinema_IsActive()) {
        warp = gScene_m_LevelID != 999;
    }
    if (gScene.m_bStreamAll) {
        warp = 1;
    }
    if (warp) {
        p->m_Data.m_CheckPointWarpID = gScene_m_LevelID;
    }
    if (!flags->multiplayer && p->m_Data.m_WeaponList[21]) {
        /* the ROM adds the scaled index first: written index + array */
        *(*CPlayer__GetFiringAmmo(p, 21) + p->m_Data.m_AmmoList) = 1;
    }
    Cinema_Disable();
    if (p->m_CharacterActor.header.m_pCurrentRegion) {
        CGameRegion__FireTriggers((sptr) p->m_CharacterActor.header.m_pCurrentRegion, 1, p);
    }
    Cinema_Enable();
}

#include "globals/engine.h"

extern void CPlayer__Respawn(CPlayer *p);
extern void Sound__PlaySpeech(s32 sound);
extern void CPlayer__Warp(CPlayer *p, s32 warp, s32 arg2, s32 respawn);

void CPlayer__StartGame_2(CPlayer *p) {
    s32 mode = (gEngineApp_m_Game.m_PlayerList.m_nPlayers >= 2) * 2;

    if (gEngineApp_m_Settings_multiplayer) {
        p->m_Data.m_Lives = 1;
    } else {
        p->m_Data.m_Lives = 3;
    }
    if (gEngineApp_m_Settings_multiplayer != 0 || (Controller_SetRumbleOption(p->m_ContState.m_pContState, 1), gEngineApp_m_Settings_multiplayer != 0)) {
        CPlayer__Respawn(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = 60.0f;
    CPlayer__Reset(p);
    p->m_ShockedTimer = 0;
    p->m_pHitMeWithCBore = 0;
    p->m_pHitMeWithChargeDart = 0;
    p->m_nNumArrowsStuckInMe = 0;
    p->m_UseLowerAmmo = 0;
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings_multiplayer == 0 && gFrameCountGameplay >= 11) {
        if (p->m_Data.m_Lives < 11) {
            Sound__PlaySpeech(gLivesSpeech[p->m_Data.m_Lives]);
        }
    }
    if (gEngineApp_m_Settings_multiplayer != 0) {
        CPlayer__GiveStartingWeapons(p);
    }
    p->m_bEnteringDeathmatch = 0;
    CPlayer__Warp(p, p->m_Data.m_CheckPointWarpID, mode, 1);
}

#include "protos/player.h"

void CPlayer__LoseLife_2(CPlayer *p) {
    s32 mode = (gEngineApp_m_Game.m_PlayerList.m_nPlayers >= 2) * 2;
    GameSettings *flags = &gEngineApp_m_Settings;

    if (!(flags->cheatFlags & 2) || flags->multiplayer != 0) {
        p->m_Data.m_Lives--;
    }
    if (p->m_Data.m_Lives <= 0) {
        if (gEngineApp_m_Settings_multiplayer == 0 || (s8)p->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted != 0 || p->m_pOptions->m_TagMode != 0) {
            CPlayer__SetState(p, p, 15);
        }
        return;
    }
    CPlayer__Warp(p, p->m_Data.m_CheckPointWarpID, mode, 1);
    if (gEngineApp_m_Settings_multiplayer != 0 || (Controller_SetRumbleOption(p->m_ContState.m_pContState, 1), gEngineApp_m_Settings_multiplayer != 0)) {
        CPlayer__Respawn(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = 60.0f;
    CPlayer__Reset(p);
    p->m_ShockedTimer = 0;
    p->m_pHitMeWithCBore = 0;
    p->m_pHitMeWithChargeDart = 0;
    p->m_nNumArrowsStuckInMe = 0;
    p->m_UseLowerAmmo = 0;
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings_multiplayer == 0 && gFrameCountGameplay >= 11) {
        if (p->m_Data.m_Lives < 11) {
            Sound__PlaySpeech(gLivesSpeech[p->m_Data.m_Lives]);
        }
    }
    if (gEngineApp_m_Settings_multiplayer != 0) {
        CPlayer__GiveStartingWeapons(p);
    }
}

#include "protos/scene.h"

extern s32 Random_Next(void);

static __inline__ CIntelligence *findDef(s32 kind, s32 character) {
    return CScene__FindIntelligence(&gScene, kind, character);
}

void CPlayer__SetCharacter_2(CPlayer *p, s32 character) {
    CIntelligence *def;
    s32 modelId;

    p->m_Data.m_nDeathmatchCharacter = character;
    if (p->m_pOptions->m_TagMode == 1) {
        character = 11;
    }
    def = findDef(11, character);
    if (def == NULL) {
        def = findDef(11, -1);
        if (def == NULL) {
            def = findDef(-1, -1);
        }
    }
    p->m_CharacterActor.header.m_pIntelligence = def;
    p->m_CharacterActor.m_vScale.x = def->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = def->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = def->m_Player.m_ScaleZ;
    p->m_Data.m_Health = def->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    if (p->m_Data.m_nDeathmatchCharacter == 7) {
        p->m_CharacterActor.m_AI.m_nTexture = Random_Next() % 18;
    } else {
        p->m_CharacterActor.m_AI.m_nTexture = -1;
    }
    CPlayer__GiveStartingWeapons(p);
    modelId = 0x66;
    if (gEngineApp_m_Settings.multiplayer) {
        if (gEngineApp_m_Settings_match_tag[0] != 0 && p->m_pOptions->m_TagMode != 0) {
            modelId = gTagModeModel;
        } else {
            modelId = gCharacterModels[p->m_CharacterActor.header.m_pIntelligence->m_Id];
        }
    }
    CScene__SetInstanceObjectType(&gScene, p, modelId, p->m_CharAnim);
}

#include "structs/controller.h"
#include "protos/list.h"

typedef struct PlayerList {
    /* 0x00 */ CPlayer **handle;
    /* 0x04 */ CPlayer *players;
    /* 0x08 */ s32 count;
    /* 0x0C */ u8 list0[0x14];
    /* 0x20 */ u8 list1[0x14];
} PlayerList;
extern CPlayer **CCache__Allocate(s32 heap, s32 size, s32 tag, char *name);
extern void CCache__Deallocate(sptr heap, CPlayer **handle);
extern void CCache__Flush(s32);
extern void Controller_SetRumbleOptionAll(s32);
extern void memset(void *p, s32 c, sptr n); /* memset */
extern void CPlayerContState__Init(void *, ControllerProfile *);

/* CPlayer__Construct, inlined. */
static __inline__ void initPlayer(CPlayer *p, s32 index, CPlayerOptions *options) {
    p->m_bActive = 0;
    CPlayerContState__Init(&p->m_ContState, &gControllers[index]);
    p->m_nController = index;
    p->m_pOptions = options;
    p->m_pLoopingSound_WeaponBackground = 0;
    p->m_pLoopingSound_WeaponFiring = 0;
    p->m_Data.m_CheckPointWarpID = 0;
    p->m_Data.m_SavePointWarpID = 0;
    p->m_bLevelNotCompleteExit = 0;
    CPlayer__Reset(p);
}

void CPlayerPool__NewGame_2(PlayerList *list, s32 count, s32 arg2) {
    GameSettings *flags;
    CPlayerOptions *options;
    CPlayer *p;
    s32 i;

    CList__Construct(list->list0, T2_OFFSETOF(CPlayer, m_pLast), T2_OFFSETOF(CPlayer, m_pNext));
    CList__Construct(list->list1, T2_OFFSETOF(CPlayer, m_pLast), T2_OFFSETOF(CPlayer, m_pNext));
    Controller_SetRumbleOptionAll(0);
    CCache__Flush(0);
    flags = &gEngineApp_m_Settings;
    if (list->handle != NULL) {
        CCache__Deallocate(0, list->handle);
        list->handle = NULL;
        list->players = NULL;
        list->count = 0;
    }
    if (count == 0) {
        return;
    }
    list->count = count;
    list->handle = CCache__Allocate(0, count * sizeof(CPlayer), 0x23, "Players");
    list->players = *list->handle;
    memset(list->players, 0, count * sizeof(CPlayer));
    for (i = 0; i < list->count; i++) {
        CList__AddTail(list->list0, &list->players[i]);
    }
    options = &flags->defaultSlot;
    if (flags->multiplayer) {
        for (i = 0; i < list->count; i++) {
            p = &list->players[i];
            options = &flags->slots[i];
            initPlayer(p, i, options);
            if (options->m_bActive) {
                CPlayer__NewGame_2(p, options->m_nCharacter, 1);
                CList__Remove(list->list0, p);
                CList__AddHead(list->list1, p);
            }
        }
    } else {
        p = list->players;
        initPlayer(p, options->m_nController, options);
        CPlayer__NewGame_2(p, options->m_nCharacter, arg2);
        CList__Remove(list->list0, p);
        CList__AddHead(list->list1, p);
    }
    gEngineApp_m_Settings.match.tagPhase = 1;
}

void CPlayerPool__Construct_2(CPlayerPool *pl) {
    memset(pl, 0, sizeof(CPlayerPool));
    CList__Construct(&pl->m_IdleList, T2_OFFSETOF(CPlayer, m_pLast), T2_OFFSETOF(CPlayer, m_pNext));
    CList__Construct(&pl->m_ActiveList, T2_OFFSETOF(CPlayer, m_pLast), T2_OFFSETOF(CPlayer, m_pNext));
    pl->m_pmePlayers = 0;
    pl->m_Players = 0;
    pl->m_nPlayers = 0;
    pl->m_bCinema = 0;
    pl->m_bCinemaStarted = 0;
}

extern void SavedPlayers_Deactivate(void);
extern void SavedPlayers_Restore(void);

void CPlayerPool__LevelStart_2(CPlayerPool *list) {
    CPlayer *p;

    if (gEngineApp_m_Settings.mode == 13 && SavedPlayers_IsActive() != 0) {
        SavedPlayers_Restore();
        SavedPlayers_Deactivate();
    }
    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        CPlayer__LevelStart_2(p);
        CPlayer__UpdateOrbitCamera(p, p);
        Cinema_UpdateCameraIfActive(p->m_pCamera);
    }
}

void CPlayer__Construct_2(CPlayer *p, s32 index, void *options) {
    p->m_bActive = 0;
    CPlayerContState__Init(&p->m_ContState, &gControllers[index]);
    p->m_nController = index;
    p->m_pOptions = options;
    p->m_pLoopingSound_WeaponBackground = 0;
    p->m_pLoopingSound_WeaponFiring = 0;
    p->m_Data.m_CheckPointWarpID = 0;
    p->m_Data.m_SavePointWarpID = 0;
    p->m_bLevelNotCompleteExit = 0;
    CPlayer__Reset(p);
}

void CPlayer__Activate_2(CPlayer *p, s32 arg1) {
    p->m_pOptions->m_bActive = 1;
    CPlayer__NewGame_2(p, arg1, 1);
    CAI__SetMode(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 2);
    CPlayer__Warp(p, gEngineApp_m_Settings.matchMode, 0, 1);
}

void CPlayer__ResetHealth_2(CPlayer *p) {
    GameSettings *flags = &gEngineApp_m_Settings;

    if (flags->multiplayer != 0 || (Controller_SetRumbleOption(p->m_ContState.m_pContState, 1), flags->multiplayer != 0)) {
        CPlayer__Respawn(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = 60.0f;
    CPlayer__Reset(p);
    p->m_ShockedTimer = 0;
    p->m_pHitMeWithCBore = 0;
    p->m_pHitMeWithChargeDart = 0;
    p->m_nNumArrowsStuckInMe = 0;
    p->m_UseLowerAmmo = 0;
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings.multiplayer == 0 && gFrameCountGameplay >= 11) {
        if (p->m_Data.m_Lives < 11) {
            Sound__PlaySpeech(gLivesSpeech[p->m_Data.m_Lives]);
        }
    }
    if (gEngineApp_m_Settings_multiplayer != 0) {
        CPlayer__GiveStartingWeapons(p);
    }
}

extern void LoopSounds_Stop(void *pool, sptr sound);

/* Stops the player's two looping sounds. */
void CPlayer__StopLoopingSounds_2(CPlayer *p) {
    LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponBackground);
    p->m_pLoopingSound_WeaponBackground = 0;
    LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponFiring);
    p->m_pLoopingSound_WeaponFiring = 0;
}
