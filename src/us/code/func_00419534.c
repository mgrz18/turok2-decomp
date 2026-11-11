#include "common.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"



s32 DeathmatchMenu_OpenWeapons(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gWeaponsMenuDesc);
    return 1;
}
