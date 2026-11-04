/* Second VM module copy of func_0042E29C (same source, linked 0x0042E1BC). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

typedef struct GameState {
    s32 state;
} GameState;

extern s32 D_80119878; /* player count */
extern u8 D_8011AAF1;  /* multiplayer */
extern GameState D_8011AC88;
extern u32 D_800B6D20;
extern s16 D_800AE2B4[];
extern f32 D_800A6AC8; /* 60.0f, in the engine's .rodata */

extern void func_0024E5F0(CPlayer *p, CPlayer *q, s32);
extern void func_00285BD0(s32 controller, s32 state);
extern void func_002540E4(CPlayer *p);
extern void func_00248340(CPlayer *p);
extern void func_0027580C(s32 sound);
extern void func_00248A8C(CPlayer *p);
extern void func_00248BA8(CPlayer *p, s32 warp, s32 arg2, s32 respawn);

void func_004671BC(CPlayer *p) {
    s32 mode = (D_80119878 >= 2) * 2;
    GameSettings *flags = &D_8011AAD8;

    if (!(flags->cheatFlags & 2) || flags->multiplayer != 0) {
        p->m_Data.m_Lives--;
    }
    if (p->m_Data.m_Lives <= 0) {
        if (D_8011AAF1 == 0 || (s8)p->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted != 0 || p->m_pOptions->m_TagMode != 0) {
            func_0024E5F0(p, p, 15);
        }
        return;
    }
    func_00248BA8(p, p->m_Data.m_CheckPointWarpID, mode, 1);
    if (D_8011AAF1 != 0 || (func_00285BD0(p->m_ContState.m_pContState, 1), D_8011AAF1 != 0)) {
        func_002540E4(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = D_800A6AC8;
    func_00248340(p);
    p->m_ShockedTimer = 0;
    p->m_pHitMeWithCBore = 0;
    p->m_pHitMeWithChargeDart = 0;
    p->m_nNumArrowsStuckInMe = 0;
    p->m_UseLowerAmmo = 0;
    if (D_8011AC88.state == 13 && D_8011AAF1 == 0 && D_800B6D20 >= 11) {
        if (p->m_Data.m_Lives < 11) {
            func_0027580C(D_800AE2B4[p->m_Data.m_Lives]);
        }
    }
    if (D_8011AAF1 != 0) {
        func_00248A8C(p);
    }
}
