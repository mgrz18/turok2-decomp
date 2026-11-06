#include "common.h"
#include "globals/menu_desc.h"

s32 MenuPool_OpenChildMenu_2(s32, s32, s32);


s32 func_004527B8(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (s32) &gOptionsMenuDesc_2);
    return 1;
}
