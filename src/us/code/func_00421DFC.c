#include "common.h"
#include "structs/menu.h"

extern s32 D_800B490C;
extern s32 D_80130990;

s32 PakMenu_UpdateCancel(MenuItem *arg0) {
    if (D_80130990 != 0) {
        arg0->table = &D_800B490C;
    }
    return 0;
}
