#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B608C[];
extern char D_800B60A4[];
extern char D_800B60BC[];
extern char D_800B60D4[];
extern char D_800B60EC[];
extern char D_800B6104[];
extern char D_800B611C[];
extern char D_800B6134[];
extern char D_800B614C[];

/* Names setting 0x2D. */
s32 func_004521C0(MenuItem *item) {
    switch (D_8011AAD8.teamSkill) {
    case 0:
        item->text = D_800B608C;
        break;
    case 1:
        item->text = D_800B60A4;
        break;
    case 2:
        item->text = D_800B60BC;
        break;
    case 3:
        item->text = D_800B60D4;
        break;
    case 4:
        item->text = D_800B60EC;
        break;
    case 5:
        item->text = D_800B6104;
        break;
    case 6:
        item->text = D_800B611C;
        break;
    case 7:
        item->text = D_800B6134;
        break;
    case 8:
        item->text = D_800B614C;
        break;
    }
    return 0;
}
