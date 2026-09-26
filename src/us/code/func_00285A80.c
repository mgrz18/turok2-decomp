#include "common.h"
#include "m2c_macros.h"

extern u8 D_800F5EA0[];

void func_00285A80(s32 arg0) {
    *(((u8 *)(D_800F5EA0 + arg0))) = 0;
}
