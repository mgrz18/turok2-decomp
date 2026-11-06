#include "common.h"
#include "globals/engine.h"

extern s32 CEngineApp__TimedModeSwitch(s32, f32, s32, s32);

void EngineMode03_ExpansionPak_Retrace(s32 arg0) {
    if (CEngineApp__TimedModeSwitch(arg0, 4.0f, 4, 4) != 0) {
        gShowExpansionPakScreen = 0;
    }
}
