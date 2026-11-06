/* WRONG CUT: the real function starts at 0x0045AEA8 (size 0xD8 = 216): splat
 * cut its first two words (lui/lw D_800C201C) as GameNoteMenu_UpdateLevel_2. */
#include "common.h"
#include "structs/menu.h"
#include "structs/save.h"

/* Set the menu item's text from a state of the current object. */

extern char D_800B4ABC[];
extern char D_800B4AB4[];
extern char D_800B4A40[];
extern char D_800B4A50[];
extern char D_800B4A60[];
extern char D_800B4A70[];
extern char D_800B4A88[];
extern char D_800B4A9C[];

s32 Level_GetHub(s32 id);

s32 GameNoteMenu_UpdateLevel_2(MenuItem *item) {
    switch (Level_GetHub(D_800C201C->player.m_CheckPointWarpID)) {
    case -1:
    default:
        item->text = D_800B4ABC;
        break;
    case 0:
        item->text = D_800B4AB4;
        break;
    case 1:
        item->text = D_800B4A40;
        break;
    case 2:
        item->text = D_800B4A50;
        break;
    case 3:
        item->text = D_800B4A60;
        break;
    case 4:
        item->text = D_800B4A70;
        break;
    case 5:
        item->text = D_800B4A88;
        break;
    case 6:
        item->text = D_800B4A9C;
        break;
    }
    return 0;
}
