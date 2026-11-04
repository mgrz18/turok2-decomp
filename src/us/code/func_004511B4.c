#include "common.h"
#include "structs/menu.h"

s32 func_00416644(s32, s32, s32, s32, s32);
s32 func_00275904(s32);                     /* extern */
s32 func_00421170();                            /* extern */

extern s32 D_0043B5C8;
extern s32 D_80119E2C;

s32 func_004511B4(s32 arg0, Menu *arg1) {
    s32 temp_s1;

    temp_s1 = arg1->player;
    func_00421170();
    func_00275904(-1);
    func_00416644((s32) &D_80119E2C, (s32) &D_0043B5C8, temp_s1, arg1->controller, 0);
    return 1;
}
