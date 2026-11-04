#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

/* Writes setting unk20 (or "none" when 0) over the end of the item's text. */
s32 func_004519E8(MenuItem *item) {
    GameSettings *s = &D_8011AAD8;

    if (s->dmTimeLimit != 0) {
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "  %02d", s->dmTimeLimit);
    } else {
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "none");
    }
    return 0;
}
