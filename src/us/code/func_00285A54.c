#include "common.h"
#include "m2c_macros.h"

extern u8 D_800F5EA0[];

u8 func_00285A54(s32 arg0) {
    return *(((u8 *)(D_800F5EA0 + arg0)));
}
