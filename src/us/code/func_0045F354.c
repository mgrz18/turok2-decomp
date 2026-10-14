#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002052D8(M2C_UNK, s32);                /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */
extern s32 D_800C2220;
extern s32 D_800C2224;
extern s32 D_800C2228;

void func_0045F354(void) {
    func_002053A8(0);
    if (D_800C2228 != 0) {
        func_002052D8(0, D_800C2228);
        D_800C2228 = 0;
        D_800C2224 = 0;
    }
    D_800C2220 = 0;
}
