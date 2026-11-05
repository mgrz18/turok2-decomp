#include "common.h"
#include "structs/menu.h"

s32 func_00285A68(s8);                          /* extern */

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void func_00451154(Menu *arg0) {
    func_00285A68(arg0->controller->port);
}
