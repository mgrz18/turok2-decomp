#include "common.h"
#include "m2c_macros.h"
#include "protos/iset.h"
#include "protos/cache.h"


void CCache__LoadPersistantSubBlock(s32 arg0, sptr arg1, s32 arg2, s32 arg3, sptr arg4, s32 arg5, sptr arg6) {
    s32 sp20;

    CCache__Request(0, CISet__GetROMAddressAndSize(arg1, arg3, arg2, &sp20), sp20, 0x1B, arg4, arg5, arg6, 1);
}
