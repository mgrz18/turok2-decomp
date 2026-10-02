#include "common.h"
#include "m2c_macros.h"

extern s32 D_80130940;
extern s32 D_80130998;
extern s32 D_801309AC;

void func_0045C050(s32 arg0, void *arg1) {
    if ((D_801309AC == 0) && (D_80130998 != 0) && ((u32) M2C_FIELD(arg1, u32 *, 0x10) < 2U)) {
        D_801309AC = 1;
        D_80130940 = 1;
    }
}
