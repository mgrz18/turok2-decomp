#include "common.h"

extern void CEngineApp__StartWarp(sptr, s32);

void EngineMode04_Cinema_Init(sptr arg0) {
    CEngineApp__StartWarp(arg0, 0x64);
}
