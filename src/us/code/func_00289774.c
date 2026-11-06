#include "common.h"
#include "m2c_macros.h"

extern void CEngineApp__StartWarp(s32, s32);
extern s32 D_800B6D60;

void EngineMode09_Cinema_Init(s32 arg0) {
    D_800B6D60 += 1;
    CEngineApp__StartWarp(arg0, 0x6F);
}
