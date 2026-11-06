#include "common.h"
#include "structs/game.h"

/* The multiplayer match state at gEngineApp_m_Settings_match: the fields this resets. */

/* The game/menu context this runs on. */

s32 CScene__FindWarpDests(s32, s32, s32, s32, s32);
extern s32 gScene;

void CEngineApp__StartGameAtWarp(CEngineApp *arg0, s32 arg1) {
    u8 sp18[0x18];
    s32 var_s0;

    var_s0 = arg1;
    if (CScene__FindWarpDests((s32) &gScene, -1, var_s0, (s32) sp18, 1) == 0) {
        var_s0 = 0;
    }
    gEngineApp_m_Settings_match.frames = 0;
    gEngineApp_m_Settings_match.seconds = 0;
    gEngineApp_m_Settings_match.minutes = 0;
    gEngineApp_m_Settings_match.hours = 0;
    gEngineApp_m_Settings_match.unk44 = 0;
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.matchMode = var_s0;
    arg0->m_Settings.warpFlags = 1;
    arg0->m_Settings.nextMode = 0xD;
}
