#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B617C[];
extern char D_800B6164[];

/* Names setting 0x29. */
s32 func_004517BC(MenuItem *item) {
    switch (D_8011AAD8.tagCrosshairs) {
    case 0:
        item->text = D_800B617C;
        break;
    case 1:
        item->text = D_800B6164;
        break;
    }
    return 0;
}
