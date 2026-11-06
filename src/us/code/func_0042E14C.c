/* CPlayer code: CPlayer__StartGame, 0x0042E14C-0x0042E29C (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/game.h"
#include "structs/player.h"
#include "globals/game.h"

extern s32 gEngineApp_m_Game_m_PlayerList_m_nPlayers; /* player count */
extern u32 gFrameCountGameplay;
extern s16 gLivesSpeech[];
extern f32 D_800A6AC4; /* 60.0f, in the engine's .rodata */

extern void Controller_SetRumbleOption(s32 controller, s32 state);
extern void CPlayer__Respawn(CPlayer *p);
extern void CPlayer__Reset(CPlayer *p);
extern void Sound__PlaySpeech(s32 sound);
extern void CPlayer__GiveStartingWeapons(CPlayer *p);
extern void CPlayer__Warp(CPlayer *p, s32 warp, s32 arg2, s32 respawn);

void CPlayer__StartGame(CPlayer *p) {
    s32 mode = (gEngineApp_m_Game_m_PlayerList_m_nPlayers >= 2) * 2;

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
    p->m_InvincibilityTime = D_800A6AC4;
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
