#include "common.h"
#include "m2c_macros.h"

extern s32 sPakConfirmDone;
extern s32 gPakConfirmKind;

void PakConfirmMenu_InitRepair_2(void) {
    gPakConfirmKind = 0;
    sPakConfirmDone = 0;
}
