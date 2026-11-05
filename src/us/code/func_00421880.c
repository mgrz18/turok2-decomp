#include "common.h"
#include "structs/menu.h"

extern s32 D_800C202C;
extern s32 D_8011B108;

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 func_00421880(Menu *arg0) {
    if ((arg0->controller->buttons & 0x1000) && (D_800C202C == 0)) {
        D_8011B108 = 1;
        D_800C202C = 1;
    }
    return D_8011B108;
}
