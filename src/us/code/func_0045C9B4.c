#include "common.h"
#include "m2c_macros.h"

extern s32 sPakConfirmDone;
extern s32 gPakConfirmKind;

void PakConfirmMenu_InitOverwrite_2(void) {
    gPakConfirmKind = 3;
    sPakConfirmDone = 0;
}
