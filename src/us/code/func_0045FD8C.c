#include "common.h"
#include "m2c_macros.h"

u32 Pak_BytesToPages_2(s32 arg0) {
    return (u32) (arg0 + 0xFF) >> 8;
}
