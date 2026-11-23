#include "common.h"
#include "globals/pak.h"

s32 Pak_IsRumblePak_2(s32 arg0) {
    return *(s32 *) ((s8 *) gPakState + arg0 * 4) == 2; /* gPakState[arg0], indexed by bytes as the ROM does */
}
