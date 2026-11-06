#include "common.h"

s32 MenuPool_OpenChildMenu_2(s32, s32, s32);

extern s32 D_0043A150;

s32 func_00452A7C(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (s32) &D_0043A150);
    return 1;
}
