#include "common.h"
#include "structs/menu.h"

void func_00285A68(s8);                             /* extern */
extern s32 D_800C2020;

void func_004181C8(Menu *arg0) {
    func_00285A68(arg0->controller->port);
    D_800C2020 = 0;
}
