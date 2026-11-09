#include "common.h"

s32 CEngineApp__StartWarp(sptr, s32);                /* extern */
s32 Demo_End();                            /* extern */

void EngineMode12_Cinema_Init(sptr arg0) {
    Demo_End();
    CEngineApp__StartWarp(arg0, 0x8D);
}
