#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"

s32 MenuPool_OpenMenu_2(s32, s32, s32, s32, s32);
s32 Sound__StopAllExcept(s32);                     /* extern */
s32 func_0045A170();                            /* extern */

s32 CharacterMenu_LoadSelect_2(s32 arg0, Menu *arg1) {
    sptr temp_s1;

    temp_s1 = arg1->player;
    func_0045A170();
    Sound__StopAllExcept(-1);
    MenuPool_OpenMenu_2((sptr) &gEngineApp_m_Game_m_MenuPool, (sptr) &gPakLoadProfileMenuDesc_2, temp_s1, arg1->controller, 0);
    return 1;
}
