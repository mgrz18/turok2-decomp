#include "common.h"
#include "m2c_macros.h"

s32 CEngineApp__UpdateCinemaMode(s32, s32, s32);

void EngineMode12_Cinema_Retrace(s32 arg0) {
    CEngineApp__UpdateCinemaMode(arg0, 0xD, 0xD);
}
