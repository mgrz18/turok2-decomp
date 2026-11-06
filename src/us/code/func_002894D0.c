#include "common.h"

extern s32 CEngineApp__TimedModeSwitch(s32, f32, s32, s32);

void EngineMode02_DemoWarning_Retrace(s32 arg0) {
    CEngineApp__TimedModeSwitch(arg0, 14.0f, 5, 5);
}
