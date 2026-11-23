#include "common.h"
#include "globals/pak.h"


s32 Pak_IsRumblePak(s32 arg0) {
    return gPakState[arg0] == 2;
}
