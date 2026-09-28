#include "common.h"
#include "m2c_macros.h"

extern s32 func_0026EE40(void *);

s32 func_0026E9F4(u8 *arg0) {
    s32 r = 0;

    if (func_0026EE40(arg0 + 0x20) != 0 || func_0026EE40(arg0 + 0x2C) != 0) {
        r = 1;
    }
    return r;
}
