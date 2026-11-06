#include "common.h"

s32 MenuPool_OpenChildMenu(s32, s32, s32);

extern u8 D_0043C864[];

s32 func_00452A50(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, (s32) D_0043C864);
    return 1;
}
