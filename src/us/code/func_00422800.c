#include "common.h"
#include "m2c_macros.h"

extern s32 gPakMessageKind;

void PakMessage_InitNoteCorrupt(void) {
    gPakMessageKind = 0xB;
}
