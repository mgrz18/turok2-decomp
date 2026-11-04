#include "common.h"
#include "structs/pak.h"

s32 func_002052D8(s32, s32);                /* extern */
s32 func_002053A8(s32);                     /* extern */
extern s32 D_800C2220;
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
