#include "common.h"
#include "m2c_macros.h"

extern s32 gPakMessageKind;

void PakMessage_InitDeleteFailed_2(void) {
    gPakMessageKind = 0xE;
}
