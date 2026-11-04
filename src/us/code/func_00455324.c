#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B4008[];
extern char D_800B4020[];
extern char D_800B4038[];

/* Shows the HUD alpha setting in sixteenths: two fixed lines for 0 and 16, otherwise the number. */
s32 func_00455324(MenuItem *item) {
    s32 value = D_8011AAD8.hudAlphaSetting.value >> 4;

    if (value == 0) {
        item->text = D_800B4008;
    } else if (value == 16) {
        item->text = D_800B4020;
    } else {
        item->text = D_800B4038;
        func_0029E3E0(&item->text[func_004160F0(item) - 2], "%02d", value);
    }
    return 0;
}
