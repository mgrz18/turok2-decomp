#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"

/* Only the field read here: a timer far into this (large) state. */

extern s32 CEngineApp__SetupFadeTo(s32, s32);
extern f32 D_800A9E14[];

void EngineMode08_MainMenu_Retrace(CEngineApp *arg0) {
    if (gMergedController.buttons != 0) {
        arg0->m_Settings.modeTimer = 0.0f;
    }
    if (arg0->m_Settings.modeTimer > D_800A9E14[0]) {
        CEngineApp__SetupFadeTo((s32) arg0, 7);
    }
}
