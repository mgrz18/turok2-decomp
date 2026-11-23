#include "common.h"
#include "m2c_macros.h"

s32 BitAddr_ToPointer(s32 arg0) {
    return (arg0 & 0xF0000000) | ((u32) (arg0 & 0x0FFFFFFF) >> 3);
}
