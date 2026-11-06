/* CPlayer code: CPlayer__ResetHealth, 0x0042EA00-0x0042EB04 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "globals/player.h"

extern f32 D_800A70D8; /* 60.0f, in the engine's .rodata */

extern void Controller_SetRumbleOption(s32 controller, s32 state);
extern void CPlayer__Respawn(CPlayer *p);
extern void CPlayer__Reset(CPlayer *p);
extern void Sound__PlaySpeech(s32 sound);
extern void CPlayer__GiveStartingWeapons(CPlayer *p);

void CPlayer__ResetHealth(CPlayer *p) {
    GameSettings *flags = &gEngineApp_m_Settings;

    if (flags->multiplayer != 0 || (Controller_SetRumbleOption(p->m_ContState.m_pContState, 1), flags->multiplayer != 0)) {
        CPlayer__Respawn(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = D_800A70D8;
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
