#include "common.h"
#include "m2c_macros.h"

s32 CEngineApp__UpdateCinemaMode(sptr, s32, s32);

void EngineMode05_Cinema_Retrace(sptr arg0) {
    CEngineApp__UpdateCinemaMode(arg0, 7, 7);
}
