#include "common.h"
#include "m2c_macros.h"

s32 func_002603D0(void *arg0) {
    return (M2C_FIELD(arg0, u16 *, 0x7E) & 0x3E0) != 0;
}
