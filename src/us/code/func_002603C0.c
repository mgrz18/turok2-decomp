#include "common.h"
#include "m2c_macros.h"

s32 func_002603C0(void *arg0) {
    return (M2C_FIELD(arg0, u16 *, 0x7E) & 0x1F) != 0;
}
