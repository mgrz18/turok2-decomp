/* WRONG CUT: the real function is 0x0045CC58-0x0045CCC4 (size 0x6C = 108): the
 * switch's case blocks were cut as func_0045CC90, func_0045CCA0 and
 * func_0045CCB0 (the last one already in src/, it must go). */
#include "common.h"
#include "structs/menu.h"

/* Set the menu item's text from the game mode gPakConfirmKind. */

extern s32 gPakConfirmKind;
extern char D_800B5310[];
extern char D_800B52DC[];

s32 PakConfirmMenu_UpdateYes_2(MenuItem *item) {
    switch (gPakConfirmKind) {
    case 0:
    default:
        item->text = D_800B5310;
        break;
    case 3:
        item->text = D_800B5310;
        break;
    case 4:
        item->text = D_800B5310;
        break;
    case 1:
    case 2:
        item->text = D_800B52DC;
        break;
    }
    return 0;
}
