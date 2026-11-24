#include "common.h"
#include "globals/pak.h"


s32 Pak_GetStatus(s32 i) {
    if (gPakState[i] != 3) {
        return -2;
    }
    return gPakError[i];
}
