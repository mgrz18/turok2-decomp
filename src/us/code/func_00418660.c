#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

extern s8 D_8011AAFF; /* D_8011AAD8 + 0x27: level */
extern char D_800B5F3C[];
extern char D_800B61D8[];
extern char D_800B61F0[];
extern char D_800B6208[];
extern char D_800B6220[];
extern char D_800B6238[];
extern char D_800B6250[];

/* Label of the level item. */
s32 func_00418660(MenuItemTmpl *a) {
    switch (D_8011AAFF) {
    case 0:
        a->text = D_800B5F3C;
        break;
    case 1:
        a->text = D_800B61D8;
        break;
    case 2:
        a->text = D_800B61F0;
        break;
    case 3:
        a->text = D_800B6208;
        break;
    case 4:
        a->text = D_800B6220;
        break;
    case 5:
        a->text = D_800B6238;
        break;
    case 6:
        a->text = D_800B6250;
        break;
    }
    return 0;
}
