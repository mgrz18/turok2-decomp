#include "common.h"

s32 MenuPool_OpenChildMenu_2(s32, s32, s32);

extern u8 D_00438E94[];

s32 func_00452494(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (s32) D_00438E94);
    return 1;
}
