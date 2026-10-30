/* WRONG CUT: real function 0x00419260, size 0xBC (through 0x0041931C): splat cut the last
 * case and the return off as func_0041930C. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern s8 D_8011AB05; /* D_8011AAD8 + 0x2D */
extern char D_800B608C[];
extern char D_800B60A4[];
extern char D_800B60BC[];
extern char D_800B60D4[];
extern char D_800B60EC[];
extern char D_800B6104[];
extern char D_800B611C[];
extern char D_800B6134[];
extern char D_800B614C[];

s32 func_00419260(MenuItemTmpl *a) {
    switch (D_8011AB05) {
    case 0:
        a->text = D_800B608C;
        break;
    case 1:
        a->text = D_800B60A4;
        break;
    case 2:
        a->text = D_800B60BC;
        break;
    case 3:
        a->text = D_800B60D4;
        break;
    case 4:
        a->text = D_800B60EC;
        break;
    case 5:
        a->text = D_800B6104;
        break;
    case 6:
        a->text = D_800B611C;
        break;
    case 7:
        a->text = D_800B6134;
        break;
    case 8:
        a->text = D_800B614C;
        break;
    }
    return 0;
}
