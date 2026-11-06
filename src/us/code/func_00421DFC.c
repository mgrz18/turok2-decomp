#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern s32 D_800B490C;

s32 PakMenu_UpdateCancel(MenuItem *arg0) {
    if (gPakMenuAnyPak != 0) {
        arg0->table = &D_800B490C;
    }
    return 0;
}
