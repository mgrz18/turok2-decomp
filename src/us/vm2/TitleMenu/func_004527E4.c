#include "common.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"



s32 TitleMenu_EnterCheat_2(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, &gEnterCheatMenuDesc_2);
    return 1;
}
