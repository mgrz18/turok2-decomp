#include "common.h"
#include "m2c_macros.h"

extern s32 gPakMessageKind;

void PakMessage_InitPakNotFound_2(void) {
    gPakMessageKind = 1;
}
