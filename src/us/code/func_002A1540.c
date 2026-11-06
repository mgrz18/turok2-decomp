#include "common.h"
#include "m2c_macros.h"

extern u32 HostIo_ReadWord(s32);

s32 HostIo_ReadByte(s32 arg0) {
    s32 shift;

    shift = ~arg0;
    shift &= 3;
    shift *= 8;
    return (HostIo_ReadWord(arg0 & ~3) >> shift) & 0xFF;
}
