#include "common.h"

s32 MenuPool_OpenChildMenu_2(s32, s32, s32);

extern s32 D_00438B58;

s32 func_00452400(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (s32) &D_00438B58);
    return 1;
}
