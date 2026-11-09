#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/scene.h"


void func_002888BC(sptr, s32, s32);

/* The ROM bases every store on gEngineApp_m_Settings_match (the match), the settings at
 * -0x1D8 from it. */
void EngineMode01_Legal_Init(sptr arg0) {
    GameMatch *match = &gEngineApp_m_Settings.match;
    GameSettings *settings;

    match->unk44 = 0;
    settings = (GameSettings *)((u8 *)match - 0x1D8); /* gEngineApp_m_Settings */
    settings->defaultSlot.m_bActive = 1;
    settings->multiplayer = 0;
    settings->playerCount = 1;
    func_002888BC(arg0, 0, 0);
    CScene__LoadLevel(&gScene, 0, 0);
}
