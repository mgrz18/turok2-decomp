#include "common.h"
#include "structs/game.h"

/* The multiplayer match state at D_8011ACB0: the fields this resets. */

/* The game/menu context this runs on. */

s32 func_0042C194(s32, s32, s32, s32, s32);
extern s32 D_800F7078;

void func_002886D0(CEngineApp *arg0, s32 arg1) {
    u8 sp18[0x18];
    s32 var_s0;

    var_s0 = arg1;
    if (func_0042C194((s32) &D_800F7078, -1, var_s0, (s32) sp18, 1) == 0) {
        var_s0 = 0;
    }
    D_8011ACB0.frames = 0;
    D_8011ACB0.seconds = 0;
    D_8011ACB0.minutes = 0;
    D_8011ACB0.hours = 0;
    D_8011ACB0.unk44 = 0;
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.matchMode = var_s0;
    arg0->m_Settings.warpFlags = 1;
    arg0->m_Settings.nextMode = 0xD;
}
