#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B5F3C[];
extern char D_800B5F54[];
extern char D_800B5F6C[];
extern char D_800B5F84[];
extern char D_800B5F9C[];
extern char D_800B5FB4[];
extern char D_800B5FCC[];
extern char D_800B5FE4[];
extern char D_800B5FFC[];
extern char D_800B6014[];
extern char D_800B602C[];
extern char D_800B6044[];
extern char D_800B605C[];
extern char D_800B6074[];

/* Names setting 0x22. */
s32 func_00451BA0(MenuItem *item) {
    switch (D_8011AAD8.dmArena) {
    case 0:
        item->text = D_800B5F3C;
        break;
    case 1:
        item->text = D_800B5F54;
        break;
    case 2:
        item->text = D_800B5F6C;
        break;
    case 3:
        item->text = D_800B5F84;
        break;
    case 4:
        item->text = D_800B5F9C;
        break;
    case 5:
        item->text = D_800B5FB4;
        break;
    case 6:
        item->text = D_800B5FCC;
        break;
    case 7:
        item->text = D_800B5FE4;
        break;
    case 8:
        item->text = D_800B5FFC;
        break;
    case 9:
        item->text = D_800B6014;
        break;
    case 10:
        item->text = D_800B602C;
        break;
    case 11:
        item->text = D_800B6044;
        break;
    case 12:
        item->text = D_800B605C;
        break;
    case 13:
        item->text = D_800B6074;
        break;
    }
    return 0;
}
