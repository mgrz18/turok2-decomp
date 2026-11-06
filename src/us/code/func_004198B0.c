#include "common.h"

s32 MenuPool_OpenChildMenu(s32, s32, s32);

extern s32 gCheatsMenuDesc;

s32 TitleMenu_Cheats(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, (s32) &gCheatsMenuDesc);
    return 1;
}
