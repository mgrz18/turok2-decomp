#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002052D8(M2C_UNK, s32);                /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */
extern s32 D_800C2010;
extern s32 D_800C2014;
extern s32 D_800C2018;
extern s32 D_800C201C;

void func_004213CC(void) {
    if ((D_800C2010 != 0) || (D_800C2014 != 0)) {
        func_002053A8(0);
    }
    if (D_800C2010 != 0) {
        func_002052D8(0, D_800C2010);
    }
    if (D_800C2014 != 0) {
        func_002052D8(0, D_800C2014);
    }
    D_800C2010 = 0;
    D_800C2014 = 0;
    D_800C2018 = 0;
    D_800C201C = 0;
}
