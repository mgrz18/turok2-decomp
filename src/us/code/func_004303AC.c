#include "common.h"
#include "structs/game.h"

s32 func_0042C194(s32, s32, s32, s32, s32);
extern s32 D_800F7078;

void func_004303AC(CEngineApp *arg0) {
    u8 sp18[0x18];
    s32 var_s1;

    var_s1 = -1;
    if (func_0042C194((s32) &D_800F7078, -1, -1, (s32) sp18, 1) == 0) {
        var_s1 = 0;
    }
    D_8011ACB0.frames = 0;
    D_8011ACB0.seconds = 0;
    D_8011ACB0.minutes = 0;
    D_8011ACB0.hours = 0;
    arg0->m_Settings.warpFlags = 0;
    D_8011ACB0.unk44 = 0;
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.matchMode = var_s1;
    arg0->m_Settings.nextMode = 0xB;
}
