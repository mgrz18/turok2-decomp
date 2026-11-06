#include "common.h"

extern s32 gPakState[];        /* per controller-pak slot status */

s32 Pak_IsRumblePak(s32 arg0) {
    return gPakState[arg0] == 2;
}
