#include "common.h"
#include "structs/save.h"

s32 func_002052D8(s32, s32);                /* extern */
s32 func_002053A8(s32);                     /* extern */
extern s32 D_800C2010;
extern s32 D_800C2014;

void func_0045A2EC(void) {
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
