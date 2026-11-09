#include "common.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"



s32 DeathmatchMenu_OpenFragTag(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gFragTagMenuDesc);
    return 1;
}
