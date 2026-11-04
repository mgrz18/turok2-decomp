#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern s32 D_800C1F78;
extern char D_800B6600[];
extern char D_800B6618[];

/* Shows D_800C1F78: a fixed line when 0, otherwise the number. */
s32 func_0045302C(MenuItem *item) {
    if (D_800C1F78 == 0) {
        item->text = D_800B6600;
    } else {
        item->text = D_800B6618;
        func_0029E3E0(&item->text[func_004160F0(item) - 2], "%2d", D_800C1F78);
    }
    return 0;
}
