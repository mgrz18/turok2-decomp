#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern u8 D_8011AB06; /* D_8011AAD8 + 0x2E */
extern char D_800B6164[];
extern char D_800B617C[];

s32 func_0041936C(MenuItemTmpl *a) {
    switch (D_8011AB06) {
    case 0:
        a->text = D_800B617C;
        break;
    case 1:
        a->text = D_800B6164;
        break;
    }
    return 0;
}
