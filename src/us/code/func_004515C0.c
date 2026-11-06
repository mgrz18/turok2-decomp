#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B5F3C[];
extern char D_800B61D8[];
extern char D_800B61F0[];
extern char D_800B6208[];
extern char D_800B6220[];
extern char D_800B6238[];
extern char D_800B6250[];

/* Names setting 0x27. */
s32 FragTagMenu_LevelLabel_2(MenuItem *item) {
    switch (D_8011AAD8.tagArena) {
    case 0:
        item->text = D_800B5F3C;
        break;
    case 1:
        item->text = D_800B61D8;
        break;
    case 2:
        item->text = D_800B61F0;
        break;
    case 3:
        item->text = D_800B6208;
        break;
    case 4:
        item->text = D_800B6220;
        break;
    case 5:
        item->text = D_800B6238;
        break;
    case 6:
        item->text = D_800B6250;
        break;
    }
    return 0;
}
