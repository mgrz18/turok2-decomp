#include "common.h"
#include "m2c_macros.h"

void func_002810C4(void *arg0, s8 *arg1, s8 *arg2, s8 *arg3) {
    *arg1 = M2C_FIELD(arg0, u8 *, 0x508) & 0xF8;
    *arg2 = M2C_FIELD(arg0, u8 *, 0x509) & 0xF8;
    *arg3 = M2C_FIELD(arg0, u8 *, 0x50A) & 0xF8;
}
