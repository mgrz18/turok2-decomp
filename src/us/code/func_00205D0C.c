#include "common.h"
#include "m2c_macros.h"

s32 CISet__GetROMAddressAndSize(s32, s32, s32, s32);
s32 CCache__Request(s32, s32, s32, s32, s32, s32, s32, s32);

void CCache__LoadPersistantSubBlock(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 sp20;

    CCache__Request(0, CISet__GetROMAddressAndSize(arg1, arg3, arg2, (s32) &sp20), sp20, 0x1B, arg4, arg5, arg6, 1);
}
