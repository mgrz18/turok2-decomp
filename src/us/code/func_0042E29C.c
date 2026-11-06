/* CPlayer code: CPlayer__LoseLife, 0x0042E29C-0x0042E430 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

extern s32 D_80119878; /* player count */
extern u8 D_8011AAF1;  /* D_8011AAD8.multiplayer; read through D_8011AAD8 the code no longer matches */
extern u32 D_800B6D20;
extern s16 D_800AE2B4[];
extern f32 D_800A6AC8; /* 60.0f, in the engine's .rodata */

extern void CPlayer__SetState(CPlayer *p, CPlayer *q, s32);
extern void Controller_SetRumbleOption(s32 controller, s32 state);
extern void CPlayer__Respawn(CPlayer *p);
extern void CPlayer__Reset(CPlayer *p);
extern void Sound__PlaySpeech(s32 sound);
extern void CPlayer__GiveStartingWeapons(CPlayer *p);
extern void CPlayer__Warp(CPlayer *p, s32 warp, s32 arg2, s32 respawn);

void CPlayer__LoseLife(CPlayer *p) {
    s32 mode = (D_80119878 >= 2) * 2;
    GameSettings *flags = &D_8011AAD8;

    if (!(flags->cheatFlags & 2) || flags->multiplayer != 0) {
        p->m_Data.m_Lives--;
    }
    if (p->m_Data.m_Lives <= 0) {
        if (D_8011AAF1 == 0 || (s8)p->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted != 0 || p->m_pOptions->m_TagMode != 0) {
            CPlayer__SetState(p, p, 15);
        }
        return;
    }
    CPlayer__Warp(p, p->m_Data.m_CheckPointWarpID, mode, 1);
    if (D_8011AAF1 != 0 || (Controller_SetRumbleOption(p->m_ContState.m_pContState, 1), D_8011AAF1 != 0)) {
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
    if (D_8011AAD8.mode == 13 && D_8011AAF1 == 0 && D_800B6D20 >= 11) {
        if (p->m_Data.m_Lives < 11) {
            Sound__PlaySpeech(D_800AE2B4[p->m_Data.m_Lives]);
        }
    }
    if (D_8011AAF1 != 0) {
        CPlayer__GiveStartingWeapons(p);
    }
}
