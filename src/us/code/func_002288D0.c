#include "common.h"
#include "m2c_macros.h"

extern s32 D_800ACE00;
extern s32 D_800F1800;

void func_002288D0(void) {
    D_800F1800 = D_800ACE00;
    if (D_800ACE00 == 0) {
        D_800ACE00 = 0x20;
        return;
    }
    D_800ACE00 = 0;
}
