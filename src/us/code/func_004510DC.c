#include "common.h"

s32 MenuPool_OpenChildMenu(s32, s32, s32);

extern s32 D_00438590;

s32 func_004510DC(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, (s32) &D_00438590);
    return 1;
}
