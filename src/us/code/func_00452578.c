#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"

s32 MenuPool_OpenMenu_2(s32, s32, sptr, sptr, sptr);
s32 func_0045A170();                            /* extern */

s32 SingleMenu_Load_2(s32 arg0, Menu *arg1) {
    func_0045A170();
    MenuPool_OpenMenu_2((sptr) &gEngineApp_m_Game_m_MenuPool, (sptr) &gPakLoadGameMenuDesc_2, arg1->player, arg1->controller, 0);
    return 1;
}
