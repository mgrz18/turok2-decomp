#include "common.h"
#include "m2c_macros.h"

extern s32 D_800C2028;

s32 PakMenu_HasPort_2(void) {
    return ~D_800C2028 != 0;
}
