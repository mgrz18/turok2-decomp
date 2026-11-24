/* WRONG CUT: one function 0x00423CBC-0x00423D38 (size 0x7C): a switch whose cases splat cut
 * into func_00423CF4, func_00423D04, func_00423D14, func_00423D24 (jump table jtbl_800C16D0),
 * and whose shared `return 0` tail is the already-accepted func_00423D30 (8 bytes). */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"


s32 PakConfirmMenu_UpdateLine5_2(MenuItem *item) {
    switch (gPakConfirmKind) {
        case 0:
        default:
            item->text = sPakConfirmMenuText_ToAttemptRepair;
            break;
        case 1:
            item->text = sPakConfirmMenuText_Empty_B51CC;
            break;
        case 2:
            item->text = sPakConfirmMenuText_Empty_B5218;
            break;
        case 3:
            item->text = sPakConfirmMenuText_OverwriteThisNote;
            break;
        case 4:
            item->text = sPakConfirmMenuText_DeleteThisNote;
            break;
    }
    return 0;
}
