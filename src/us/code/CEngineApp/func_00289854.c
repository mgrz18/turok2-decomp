#include "common.h"

s32 CEngineApp__UpdateCinemaMode(sptr, s32, s32);       /* extern */
s32 Demo_Enable();                            /* extern */

void EngineMode10_Cinema_Retrace(sptr arg0) {
    Demo_Enable();
    CEngineApp__UpdateCinemaMode(arg0, 0xD, 8);
}
