#include "common.h"

s32 MenuPool_OpenChildMenu_2(s32, s32, s32);

extern s32 D_00439BF4;

s32 func_00452AA8(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (s32) &D_00439BF4);
    return 1;
}
