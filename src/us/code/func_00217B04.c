#include "common.h"
#include "m2c_macros.h"

s32 func_00217B04(s32 arg0) {
    return (arg0 & 0xF0000000) | ((arg0 & 0x0FFFFFFF) * 8);
}
