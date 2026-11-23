#include "common.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"



s32 TitleMenu_Cheats(sptr arg0, sptr arg1, sptr arg2) { /* sptr: menu callback args (--high-memory) */
    MenuPool_OpenChildMenu(arg2, arg1, &gCheatsMenuDesc);
    return 1;
}
