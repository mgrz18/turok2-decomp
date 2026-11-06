#include "common.h"
#include "globals/menu_desc.h"

s32 MenuPool_OpenChildMenu(s32, s32, s32);


s32 DeathmatchMenu_OpenFragTag(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, (s32) &gFragTagMenuDesc);
    return 1;
}
