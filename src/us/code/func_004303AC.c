#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/scene.h"


void func_004303AC(CEngineApp *arg0) {
    u8 sp18[0x18];
    s32 var_s1;

    var_s1 = -1;
    if (CScene__FindWarpDests((s32) &gScene, -1, -1, (s32) sp18, 1) == 0) {
        var_s1 = 0;
    }
    gEngineApp_m_Settings.match.frames = 0;
    gEngineApp_m_Settings.match.seconds = 0;
    gEngineApp_m_Settings.match.minutes = 0;
    gEngineApp_m_Settings.match.hours = 0;
    arg0->m_Settings.warpFlags = 0;
    gEngineApp_m_Settings.match.unk44 = 0;
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.matchMode = var_s1;
    arg0->m_Settings.nextMode = 0xB;
}
