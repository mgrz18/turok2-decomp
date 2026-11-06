#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"

/* The play-time clock (see CEngineApp__UpdatePlayTime.c) and a word at 0x44. */

/* The fields this sets, far into this (large) state. */

s32 func_004650B4(void *, s32, s32, void *, s32);   /* extern */

void func_004692CC(CEngineApp *arg0) {
    u8 sp18[0x18];
    s32 var_s1;

    var_s1 = -1;
    if (func_004650B4(&gScene, -1, -1, sp18, 1) == 0) {
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
