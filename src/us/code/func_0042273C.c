#include "common.h"
#include "m2c_macros.h"

extern s32 gPakMessageKind;

void PakMessage_InitCannotSave(void) {
    gPakMessageKind = 6;
}
