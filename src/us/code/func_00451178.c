#include "common.h"
#include "structs/menu.h"

s32 func_00285A68(s8);                          /* extern */
extern s32 D_800C2020;

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void func_00451178(Menu *arg0) {
    func_00285A68(arg0->controller->port);
    D_800C2020 = 0;
}
