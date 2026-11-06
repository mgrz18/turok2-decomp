#include "common.h"

extern s32 gPakState[];
extern s32 gPakError[];

s32 Pak_NeedsRepair(s32 i) {
    if (gPakState[i] != 3) {
        return 0;
    }
    return gPakError[i] == -4;
}
