/* Second VM module copy of CPlayer__ResetHealth (same source, linked 0x0042E920). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"


extern u8 D_8011AAF1; /* multiplayer */
extern u32 D_800B6D20;
extern s16 D_800AE2B4[];
extern f32 D_800A70D8; /* 60.0f, in the engine's .rodata */

extern void Controller_SetRumbleOption(s32 controller, s32 state);
extern void CPlayer__Respawn(CPlayer *p);
extern void CPlayer__Reset(CPlayer *p);
extern void Sound__PlaySpeech(s32 sound);
extern void CPlayer__GiveStartingWeapons(CPlayer *p);

void CPlayer__ResetHealth_2(CPlayer *p) {
    GameSettings *flags = &D_8011AAD8;

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
    if (D_8011AAD8.mode == 13 && D_8011AAD8.multiplayer == 0 && D_800B6D20 >= 11) {
        if (p->m_Data.m_Lives < 11) {
            Sound__PlaySpeech(D_800AE2B4[p->m_Data.m_Lives]);
        }
    }
    if (D_8011AAF1 != 0) {
        CPlayer__GiveStartingWeapons(p);
    }
}
