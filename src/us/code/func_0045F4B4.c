#include "common.h"

extern s32 gPakState;

s32 Pak_IsRumblePak_2(s32 arg0) {
    return *(((__typeof__(&gPakState))((s8 *)&gPakState + (arg0 * 4)))) == 2;
}
