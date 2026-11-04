#include "common.h"
#include "structs/menu.h"

extern s32 D_800B4B84;
extern s32 D_800B4B8C;
extern s32 D_800B4B94;
extern s32 D_80130960;
extern s32 D_801309A8;

s32 func_00422608(MenuItem *arg0) {
    if (D_80130960 != 0) {
        arg0->table = &D_800B4B84;
    } else if (D_801309A8 != 0) {
        arg0->table = &D_800B4B94;
    } else {
        arg0->table = &D_800B4B8C;
    }
    return 0;
}
