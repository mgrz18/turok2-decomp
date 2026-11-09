#include "common.h"

extern s32 CEngineApp__TimedModeSwitch(sptr, f32, s32, s32);

void EngineMode02_DemoWarning_Retrace(sptr arg0) {
    CEngineApp__TimedModeSwitch(arg0, 14.0f, 5, 5);
}
