#include "common.h"
#include "m2c_macros.h"

extern s32 sPakConfirmDone;
extern s32 gPakConfirmKind;

void PakConfirmMenu_InitOverwrite(void) {
    gPakConfirmKind = 3;
    sPakConfirmDone = 0;
}
