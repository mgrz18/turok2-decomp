#include "common.h"

extern void CEngineApp__StartWarp(sptr, s32);

void EngineMode11_Cinema_Init(sptr arg0) {
    CEngineApp__StartWarp(arg0, 0x6F);
}
