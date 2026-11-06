/* Second VM module copy of CPlayer__LoseLife (same source, linked 0x0042E1BC). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/engine.h"

extern s32 gEngineApp_m_Game_m_PlayerList_m_nPlayers; /* player count */
extern u8 gEngineApp_m_Settings_multiplayer;  /* gEngineApp_m_Settings.multiplayer; read through gEngineApp_m_Settings the code no longer matches */
extern s16 gLivesSpeech[];
extern f32 D_800A6AC8; /* 60.0f, in the engine's .rodata */

extern void CPlayer__SetState(CPlayer *p, CPlayer *q, s32);
extern void Controller_SetRumbleOption(s32 controller, s32 state);
extern void CPlayer__Respawn(CPlayer *p);
extern void CPlayer__Reset(CPlayer *p);
extern void Sound__PlaySpeech(s32 sound);
extern void CPlayer__GiveStartingWeapons(CPlayer *p);
extern void CPlayer__Warp(CPlayer *p, s32 warp, s32 arg2, s32 respawn);

void CPlayer__LoseLife_2(CPlayer *p) {
    s32 mode = (gEngineApp_m_Game_m_PlayerList_m_nPlayers >= 2) * 2;
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
    p->m_InvincibilityTime = D_800A6AC8;
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
