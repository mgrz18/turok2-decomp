#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern u8 D_8011AAF0; /* D_8011AAD8 + 0x18 */
extern char D_800C1EE0[];
extern char D_800C1EF8[];
extern char D_800C1F10[];
extern char D_800C1F28[];
extern char D_800C1F40[];
extern char D_800C1F58[];

s32 func_00419A00(MenuItemTmpl *a) {
    switch (D_8011AAF0) {
    case 0:
        a->text = D_800C1EE0;
        break;
    case 1:
        a->text = D_800C1EF8;
        break;
    case 2:
        a->text = D_800C1F10;
        break;
    case 3:
        a->text = D_800C1F28;
        break;
    case 4:
        a->text = D_800C1F40;
        break;
    case 5:
        a->text = D_800C1F58;
        break;
    }
    return 0;
}
