#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

/* Writes setting unk2B (or "none" when 0) over the end of the item's text. */
s32 func_00451F84(MenuItem *item) {
    GameSettings *s = &D_8011AAD8;

    if (s->teamScoreLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength(item) - 4], "  %02d", s->teamScoreLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength(item) - 4], "none");
    }
    return 0;
}
