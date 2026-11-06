#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B3FD8[];
extern char D_800B3FF0[];

s32 OptionsMenu_TwoPlayerLayoutText_2(MenuItem *item) {
    if (D_8011AAD8.mode == 13 && D_8011AAD8.multiplayer == 0) {
        item->flags &= ~0x1000000;
    } else {
        item->flags |= 0x1000000;
    }
    switch (D_8011AAD8.unk1B) {
    case 0:
        item->text = D_800B3FD8;
        break;
    case 1:
        item->text = D_800B3FF0;
        break;
    }
    return 0;
}
