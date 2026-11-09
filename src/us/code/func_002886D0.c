#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* The multiplayer match state at gEngineApp_m_Settings_match: the fields this resets. */

/* The game/menu context this runs on. */


void CEngineApp__StartGameAtWarp(CEngineApp *arg0, s32 arg1) {
    u8 sp18[0x18];
    s32 var_s0;

    var_s0 = arg1;
    if (CScene__FindWarpDests(&gScene, -1, var_s0, sp18, 1) == 0) {
        var_s0 = 0;
    }
    gEngineApp_m_Settings.match.frames = 0;
    gEngineApp_m_Settings.match.seconds = 0;
    gEngineApp_m_Settings.match.minutes = 0;
    gEngineApp_m_Settings.match.hours = 0;
    gEngineApp_m_Settings.match.unk44 = 0;
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.matchMode = var_s0;
    arg0->m_Settings.warpFlags = 1;
    arg0->m_Settings.nextMode = 0xD;
}
