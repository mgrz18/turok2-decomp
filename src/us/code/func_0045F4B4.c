#include "common.h"

extern s32 gPakState; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */

s32 Pak_IsRumblePak_2(s32 arg0) {
    return *(((__typeof__(&gPakState))((s8 *)&gPakState + (arg0 * 4)))) == 2;
}
