#include "common.h"
#include "m2c_macros.h"

s32 func_0025D474(s32, s32);

void func_0025FD30(s32 arg0, void *arg1) {
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        func_0025D474(arg0, (s32) arg1);
    }
}
