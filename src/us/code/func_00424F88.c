#include "common.h"
#include "structs/menu.h"

extern s32 D_800B436C;
extern s32 D_800B437C;
extern s32 D_80130A34;

s32 func_00424F88(MenuItem *arg0) {
    if (D_80130A34 != 0) {
        arg0->table = &D_800B437C;
    } else {
        arg0->table = &D_800B436C;
    }
    return 1;
}
