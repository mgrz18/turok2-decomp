#include "common.h"
#include "m2c_macros.h"

void func_00281060(void *arg0, s8 arg1, s8 arg2, s8 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    M2C_FIELD(arg0, s8 *, 0x532) = arg1;
    M2C_FIELD(arg0, s8 *, 0x536) = arg2;
    M2C_FIELD(arg0, s8 *, 0x537) = arg3;
    M2C_FIELD(arg0, s8 *, 0x539) = 0;
    M2C_FIELD(arg0, s32 *, 0x520) = 0;
    M2C_FIELD(arg0, s32 *, 0x52C) = 1;
    M2C_FIELD(arg0, s8 *, 0x538) = (s8) arg4;
    M2C_FIELD(arg0, s8 *, 0x533) = (s8) arg5;
    M2C_FIELD(arg0, s8 *, 0x534) = (s8) arg6;
    M2C_FIELD(arg0, s8 *, 0x535) = (s8) arg7;
}
