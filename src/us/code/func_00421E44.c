#include "common.h"
#include "structs/menu.h"

extern s32 D_800B49C4;
extern s32 D_800B49D0;
extern s32 D_801309B8;

s32 func_00421E44(MenuItem *arg0) {
    if (D_801309B8 != 0) {
        arg0->table = &D_800B49C4;
    } else {
        arg0->table = &D_800B49D0;
    }
    return 0;
}
