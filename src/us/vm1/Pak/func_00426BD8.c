#include "common.h"
#include "globals/pak.h"


s32 Pak_NeedsRepair(s32 i) {
    if (gPakState[i] != 3) {
        return 0;
    }
    return gPakError[i] == -4;
}
