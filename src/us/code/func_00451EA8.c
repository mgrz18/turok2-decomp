#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

/* Writes setting unk2A (or "none" when 0) over the end of the item's text. */
s32 func_00451EA8(MenuItem *item) {
    GameSettings *s = &D_8011AAD8;

    if (s->teamTimeLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", s->teamTimeLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "none");
    }
    return 0;
}
