#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

extern s32 D_800C1EA8;     /* the speed setting, 0-15 (5 is normal) */
extern char D_800B6698[];
extern char D_800B66B0[];
extern char D_800C0E9C[];  /* "%3d", earlier in the file */

/* Names the game speed: a fixed line at 1.0, otherwise the setting relative to 5. */
s32 func_0045190C(MenuItem *item) {
    if (D_8011AAD8.speed == 1.0f) {
        item->text = D_800B66B0;
    } else {
        item->text = D_800B6698;
        func_0029E3E0(&item->text[func_004160F0(item) - 3], D_800C0E9C, D_800C1EA8 - 5);
    }
    return 0;
}
