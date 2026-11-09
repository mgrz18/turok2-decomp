#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

s32 Sound__StopAllExcept(s32);                     /* extern */
s32 func_00421250();                            /* extern */

s32 CharacterMenu_LoadSelect(s32 arg0, Menu *arg1) {
    s32 temp_s1;

    temp_s1 = arg1->player;
    func_00421250();
    Sound__StopAllExcept(-1);
    MenuPool_OpenMenu((s32) &gEngineApp_m_Game_m_MenuPool, (s32) &gPakLoadProfileMenuDesc, temp_s1, arg1->controller, 0);
    return 1;
}
