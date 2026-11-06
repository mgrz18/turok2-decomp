#include "common.h"
#include "m2c_macros.h"

extern s32 sPakMessageRumbleB;
extern s32 gPakMessageKind;

void PakMessage_InitRumblePakB(void) {
    gPakMessageKind = 9;
    sPakMessageRumbleB = 1;
}
