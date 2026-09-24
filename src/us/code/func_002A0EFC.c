#include "common.h"
#include "m2c_macros.h"

s16 func_002A0448();                                /* extern */

u32 func_002A0EFC(void) {
    return (u32) ~func_002A0448() >> 0x1F;
}
