#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800C1EE0[];
extern char D_800C1EF8[];
extern char D_800C1F10[];
extern char D_800C1F28[];
extern char D_800C1F40[];
extern char D_800C1F58[];

/* Names setting 0x18. */
s32 Menu_DebugModeLabel_2(MenuItem *item) {
    switch (D_8011AAD8.unk18) {
    case 0:
        item->text = D_800C1EE0;
        break;
    case 1:
        item->text = D_800C1EF8;
        break;
    case 2:
        item->text = D_800C1F10;
        break;
    case 3:
        item->text = D_800C1F28;
        break;
    case 4:
        item->text = D_800C1F40;
        break;
    case 5:
        item->text = D_800C1F58;
        break;
    }
    return 0;
}
