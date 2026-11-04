#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B6510[];
extern char D_800B64F8[];
extern char D_800B64E0[];
extern char D_800B6528[];

/* Shows setting unk14 (0-255, centred on 0x80) in steps of 8 from -16 to 15. */
s32 func_00452D64(MenuItem *item) {
    s32 value = (D_8011AAD8.unk14 - 0x80) / 8;

    if (value == 0) {
        item->text = D_800B6510;
    } else if (value == 15) {
        item->text = D_800B64F8;
    } else if (value == -16) {
        item->text = D_800B64E0;
    } else if (value > 0) {
        item->text = D_800B6528;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], "  %2d", value);
    } else {
        item->text = D_800B6528;
        func_0029E3E0(&item->text[func_004160F0(item) - 4], " %3d", value);
    }
    return 0;
}
