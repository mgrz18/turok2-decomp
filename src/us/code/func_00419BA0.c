#include "common.h"

s32 MenuPool_OpenChildMenu(s32, s32, s32);

extern u8 gQuitMenuDesc[];

s32 PauseMenu_Quit(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, (s32) gQuitMenuDesc);
    return 1;
}
