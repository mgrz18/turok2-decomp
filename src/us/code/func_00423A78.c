#include "common.h"
#include "m2c_macros.h"

extern s32 sPakConfirmDone;
extern s32 gPakConfirmKind;

void PakConfirmMenu_InitNoSpaceAll(void) {
    gPakConfirmKind = 2;
    sPakConfirmDone = 0;
}
