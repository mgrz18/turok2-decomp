#include "common.h"

s32 MenuPool_OpenChildMenu(s32, s32, s32);

extern s32 D_0043A230;

s32 PauseMenu_Options(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, (s32) &D_0043A230);
    return 1;
}
