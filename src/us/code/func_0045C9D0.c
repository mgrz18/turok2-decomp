#include "common.h"
#include "m2c_macros.h"

extern s32 sPakConfirmDone;
extern s32 gPakConfirmKind;

void PakConfirmMenu_InitCorruptNote_2(void) {
    gPakConfirmKind = 4;
    sPakConfirmDone = 0;
}
