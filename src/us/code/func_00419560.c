#include "common.h"
#include "structs/menu.h"

s32 func_00426480(s32);

s32 func_00285A68(s8);                          /* extern */

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void func_00419560(Menu *arg0) {
    s8 temp_s0;

    temp_s0 = arg0->controller->port;
    func_00285A68(temp_s0);
    func_00426480((s32) temp_s0);
}
