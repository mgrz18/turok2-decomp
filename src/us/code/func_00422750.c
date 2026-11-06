#include "common.h"
#include "m2c_macros.h"

extern s32 gPakMessageKind;

void PakMessage_InitPakChanged(void) {
    gPakMessageKind = 7;
}
