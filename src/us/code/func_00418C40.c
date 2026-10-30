#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern s8 D_8011AAFA; /* D_8011AAD8 + 0x22 */
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

s32 func_00418C40(MenuItemTmpl *a) {
    switch (D_8011AAFA) {
    case 0:
        a->text = D_800B5F3C;
        break;
    case 1:
        a->text = D_800B5F54;
        break;
    case 2:
        a->text = D_800B5F6C;
        break;
    case 3:
        a->text = D_800B5F84;
        break;
    case 4:
        a->text = D_800B5F9C;
        break;
    case 5:
        a->text = D_800B5FB4;
        break;
    case 6:
        a->text = D_800B5FCC;
        break;
    case 7:
        a->text = D_800B5FE4;
        break;
    case 8:
        a->text = D_800B5FFC;
        break;
    case 9:
        a->text = D_800B6014;
        break;
    case 10:
        a->text = D_800B602C;
        break;
    case 11:
        a->text = D_800B6044;
        break;
    case 12:
        a->text = D_800B605C;
        break;
    case 13:
        a->text = D_800B6074;
        break;
    }
    return 0;
}
