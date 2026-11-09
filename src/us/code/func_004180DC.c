#include "common.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"



s32 PlayerSetupMenu_EditName(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, (s32) &gNameEntryMenuDesc);
    return 1;
}
