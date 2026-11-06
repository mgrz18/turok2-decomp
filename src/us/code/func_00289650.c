#include "common.h"

extern s32 CEngineApp__TimedModeSwitch(s32, f32, s32, s32);
extern s32 Demo_Enable(void);

void EngineMode07_Title_Retrace(s32 arg0) {
    Demo_Enable();
    CEngineApp__TimedModeSwitch(arg0, 20.0f, 0xD, 8);
}
