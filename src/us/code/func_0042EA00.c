/* CPlayer code: func_0042EA00, 0x0042EA00-0x0042EB04 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"


/* D_8011AAF0: the multiplayer flag D_8011AAF1 and the game state D_8011AC88. */
typedef struct GameState {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 multiplayer;     /* D_8011AAF1 */
    /* 0x002 */ u8 pad2[0x198 - 0x2];
    /* 0x198 */ s32 state;          /* D_8011AC88 */
} GameState;

extern GameState D_8011AAF0;
extern u8 D_8011AAF1; /* multiplayer */
extern u32 D_800B6D20;
extern s16 D_800AE2B4[];
extern f32 D_800A70D8; /* 60.0f, in the engine's .rodata */

extern void func_00285BD0(s32 controller, s32 state);
extern void func_002540E4(CPlayer *p);
extern void func_00248340(CPlayer *p);
extern void func_0027580C(s32 sound);
extern void func_00248A8C(CPlayer *p);

void func_0042EA00(CPlayer *p) {
    GameSettings *flags = &D_8011AAD8;

    if (flags->multiplayer != 0 || (func_00285BD0(p->m_ContState.m_pContState, 1), flags->multiplayer != 0)) {
        func_002540E4(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = D_800A70D8;
    func_00248340(p);
    p->m_ShockedTimer = 0;
    p->m_pHitMeWithCBore = 0;
    p->m_pHitMeWithChargeDart = 0;
    p->m_nNumArrowsStuckInMe = 0;
    p->m_UseLowerAmmo = 0;
    if (D_8011AAF0.state == 13 && D_8011AAF0.multiplayer == 0 && D_800B6D20 >= 11) {
        if (p->m_Data.m_Lives < 11) {
            func_0027580C(D_800AE2B4[p->m_Data.m_Lives]);
        }
    }
    if (D_8011AAF1 != 0) {
        func_00248A8C(p);
    }
}
