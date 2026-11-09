#include "common.h"
#include "m2c_macros.h"
#include "globals/engine.h"

extern void CEngineApp__StartWarp(sptr, s32);

void EngineMode09_Cinema_Init(sptr arg0) {
    gAttractRunCount += 1;
    CEngineApp__StartWarp(arg0, 0x6F);
}
