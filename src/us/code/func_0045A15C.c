#include "common.h"
#include "m2c_macros.h"

extern s32 gPakMenuPort;

s32 PakMenu_HasPort_2(void) {
    return ~gPakMenuPort != 0;
}
