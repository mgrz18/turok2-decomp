#include "common.h"
#include "m2c_macros.h"

s32 func_00275624(s32);

void func_0042C700(void *arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(arg0, s32 *, 0x18FB0) = 1;
    M2C_FIELD(arg0, s32 *, 0x18FAC) = arg2;
    M2C_FIELD(arg0, s32 *, 0x18FD4) = arg1;
    if ((arg2 == 2) && (arg1 != 0x3E7)) {
        func_00275624(0xBE0);
    }
}
