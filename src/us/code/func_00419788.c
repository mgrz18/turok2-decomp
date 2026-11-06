#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

s32 Controller_CountConnected();                                /* extern */

s32 TitleMenu_MultiUpdate(s32 arg0, Menu *arg1) {
    MenuItem *temp_a0;
    MenuItem *temp_v0;

    if (Controller_CountConnected() < 2) {
        temp_a0 = arg1->items;
        temp_a0[2].flags = temp_a0[2].flags & 0xFEFFFFFF;
        if (arg1->cursor == 2) {
            arg1->cursor = 1;
        }
    } else {
        temp_v0 = arg1->items;
        temp_v0[2].flags = temp_v0[2].flags | 0x01000000;
    }
    return 0;
}
