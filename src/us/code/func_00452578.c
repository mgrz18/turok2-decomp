#include "common.h"
#include "structs/menu.h"

s32 MenuPool_OpenMenu(s32, s32, s32, s32, s32);
s32 func_00421170();                            /* extern */
extern s32 D_0043B610;
extern s32 D_80119E2C;

s32 SingleMenu_Load_2(s32 arg0, Menu *arg1) {
    func_00421170();
    MenuPool_OpenMenu((s32) &D_80119E2C, (s32) &D_0043B610, arg1->player, arg1->controller, 0);
    return 1;
}
