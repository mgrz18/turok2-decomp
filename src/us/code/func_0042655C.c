#include "common.h"

extern s32 gPakState[];
extern s32 gPakError[];

s32 Pak_GetStatus(s32 i) {
    if (gPakState[i] != 3) {
        return -2;
    }
    return gPakError[i];
}
