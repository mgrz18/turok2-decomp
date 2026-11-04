#include "common.h"
#include "structs/game.h"

/* Only the field read here: a timer far into this (large) state. */

extern s32 func_00288C5C(s32, s32);
extern f32 D_800A9E14[];
extern s32 D_800F5D28[];

void func_00289700(CEngineApp *arg0) {
    if (D_800F5D28[0] != 0) {
        arg0->m_Settings.modeTimer = 0.0f;
    }
    if (arg0->m_Settings.modeTimer > D_800A9E14[0]) {
        func_00288C5C((s32) arg0, 7);
    }
}
