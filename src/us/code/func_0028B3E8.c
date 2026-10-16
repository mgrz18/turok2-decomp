#include "common.h"
#include "m2c_macros.h"

s32 osSendMesg(s32, s32, s32);

void func_0028B3E8(s32 arg0, s32 arg1) {
    osSendMesg(arg0 + 0x78, arg1, 1);
    osSendMesg(arg0 + 0x40, 6, 1);
}
