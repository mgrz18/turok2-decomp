#include "common.h"

extern void CEngineApp__StartWarp(s32, s32);

void EngineMode05_Cinema_Init(s32 arg0) {
    CEngineApp__StartWarp(arg0, 0x5A);
}
