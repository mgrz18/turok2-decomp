#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

extern u8 D_8011AAF3; /* D_8011AAD8 + 0x1B */
extern char D_800B3FD8[];
extern char D_800B3FF0[];

s32 OptionsMenu_TwoPlayerLayoutText(MenuItem *a) {
    if (D_8011AAD8.mode == 13 && D_8011AAD8.multiplayer == 0) {
        a->flags &= ~0x1000000;
    } else {
        a->flags |= 0x1000000;
    }
    switch (D_8011AAF3) {
    case 0:
        a->text = D_800B3FD8;
        break;
    case 1:
        a->text = D_800B3FF0;
        break;
    }
    return 0;
}
