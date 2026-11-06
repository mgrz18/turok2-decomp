#include "common.h"

s32 CEngineApp__UpdateCinemaMode(s32, s32, s32);       /* extern */
s32 Demo_Enable();                            /* extern */

void EngineMode10_Cinema_Retrace(s32 arg0) {
    Demo_Enable();
    CEngineApp__UpdateCinemaMode(arg0, 0xD, 8);
}
