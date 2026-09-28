#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;

V3 func_00280E40(u8 *arg0) {
    return *(V3 *)(arg0 + 0x24C);
}
