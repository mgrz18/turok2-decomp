/* Second VM module copy of func_0042E14C (same source, linked 0x0042E06C). */
#include "common.h"
#include "structs/player.h"

extern s32 D_80119878; /* player count */
extern u8 D_8011AAF1;  /* multiplayer */
typedef struct GameState {
    s32 state;
} GameState;

extern GameState D_8011AC88;
extern u32 D_800B6D20;
extern s16 D_800AE2B4[];
extern f32 D_800A6AC4; /* 60.0f, in the engine's .rodata */

extern void func_00285BD0(s32 controller, s32 state);
extern void func_002540E4(CPlayer *p);
extern void func_00248340(CPlayer *p);
extern void func_0027580C(s32 sound);
extern void func_00248A8C(CPlayer *p);
extern void func_00248BA8(CPlayer *p, s32 warp, s32 arg2, s32 respawn);

void func_0046706C(CPlayer *p) {
    s32 mode = (D_80119878 >= 2) * 2;

    if (D_8011AAF1) {
        p->m_Data.m_Lives = 1;
    } else {
        p->m_Data.m_Lives = 3;
    }
    if (D_8011AAF1 != 0 || (func_00285BD0(p->m_ContState.m_pContState, 1), D_8011AAF1 != 0)) {
        func_002540E4(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = D_800A6AC4;
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
    p->m_bEnteringDeathmatch = 0;
    func_00248BA8(p, p->m_Data.m_CheckPointWarpID, mode, 1);
}
