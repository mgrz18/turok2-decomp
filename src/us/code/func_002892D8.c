#include "common.h"
#include "structs/game.h"

extern u8 D_800F7078[];

void func_002888BC(s32, s32, s32);
s32 CScene__LoadLevel(void *, s32, s32);

/* The ROM bases every store on D_8011ACB0 (the match), the settings at
 * -0x1D8 from it. */
void EngineMode01_Legal_Init(s32 arg0) {
    GameMatch *match = &D_8011ACB0;
    GameSettings *settings;

    match->unk44 = 0;
    settings = (GameSettings *)((u8 *)match - 0x1D8); /* D_8011AAD8 */
    settings->defaultSlot.m_bActive = 1;
    settings->multiplayer = 0;
    settings->playerCount = 1;
    func_002888BC(arg0, 0, 0);
    CScene__LoadLevel(D_800F7078, 0, 0);
}
