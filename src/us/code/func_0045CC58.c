/* WRONG CUT: the real function is 0x0045CC58-0x0045CCC4 (size 0x6C = 108): the
 * switch's case blocks were cut as func_0045CC90, func_0045CCA0 and
 * func_0045CCB0 (the last one already in src/, it must go). */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

/* Set the menu item's text from the game mode gPakConfirmKind. */


s32 PakConfirmMenu_UpdateYes_2(MenuItem *item) {
    switch (gPakConfirmKind) {
    case 0:
    default:
        item->text = sPakConfirmMenuText_Yes;
        break;
    case 3:
        item->text = sPakConfirmMenuText_Yes;
        break;
    case 4:
        item->text = sPakConfirmMenuText_Yes;
        break;
    case 1:
    case 2:
        item->text = sPakConfirmMenuText_ManageTheControllerPak;
        break;
    }
    return 0;
}
