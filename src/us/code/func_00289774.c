#include "common.h"
#include "m2c_macros.h"

extern void CEngineApp__StartWarp(s32, s32);
extern s32 gAttractRunCount;

void EngineMode09_Cinema_Init(s32 arg0) {
    gAttractRunCount += 1;
    CEngineApp__StartWarp(arg0, 0x6F);
}
