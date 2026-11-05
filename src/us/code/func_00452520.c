#include "common.h"
#include "structs/menu.h"

s32 func_00416644(s32, s32, s32, s32, s32);

s32 func_00285A68(s8);                          /* extern */
extern s32 D_0043BC64;
extern s32 D_80119E2C;

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

s32 func_00452520(s32 arg0, Menu *arg1) {
    func_00285A68(arg1->controller->port);
    func_00416644((s32) &D_80119E2C, (s32) &D_0043BC64, (s32) arg1->player, (s32) arg1->controller, 0);
    return 1;
}
