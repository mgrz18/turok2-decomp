#include "common.h"
#include "m2c_macros.h"

extern u8 gPakWatched[];

s32 Controller_AnyPakWatched(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (gPakWatched[i] != 0) {
            return 1;
        }
    }
    return 0;
}
