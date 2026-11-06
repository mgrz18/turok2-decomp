#include "common.h"
#include "structs/menu.h"

extern s8 D_8011AAFF; /* D_8011AAD8 + 0x27: level */
extern char D_800B5F3C[];
extern char D_800B61D8[];
extern char D_800B61F0[];
extern char D_800B6208[];
extern char D_800B6220[];
extern char D_800B6238[];
extern char D_800B6250[];

/* Label of the level item. */
s32 FragTagMenu_LevelLabel(MenuItem *a) {
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
