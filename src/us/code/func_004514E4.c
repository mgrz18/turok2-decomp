#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

/* Writes setting unk26 (or "none" when 0) over the end of the item's text. */
s32 func_004514E4(MenuItem *item) {
    GameSettings *s = &D_8011AAD8;

    if (s->tagScoreLimit != 0) {
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "  %02d", s->tagScoreLimit);
    } else {
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "none");
    }
    return 0;
}
