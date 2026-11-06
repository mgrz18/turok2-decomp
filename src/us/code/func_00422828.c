#include "common.h"
#include "m2c_macros.h"
#include "globals/pak.h"


void PakMessage_InitSaveFailed(void) {
    gPakMessageKind = 0xD;
}
