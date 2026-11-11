#include "common.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"



s32 PlayerSetupMenu_EditName(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gNameEntryMenuDesc);
    return 1;
}
