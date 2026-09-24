#include "common.h"
#include "m2c_macros.h"

void func_00281054(void *arg0, s8 arg1, s8 arg2) {
    M2C_FIELD(arg0, s8 *, 0x530) = arg1;
    M2C_FIELD(arg0, s8 *, 0x531) = arg2;
}
