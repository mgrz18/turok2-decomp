#include "common.h"
#include "m2c_macros.h"
#include "protos/iset.h"
#include "protos/cache.h"


#ifdef TARGET_PORT
/* The N64 function is void, but returns CCache__Request's result in v0 and every caller
 * reads it (the handle of the sub-block). arg5 is the decompression callback. */
void *CCache__LoadPersistantSubBlock(s32 arg0, sptr arg1, s32 arg2, s32 arg3, sptr arg4, sptr arg5, sptr arg6) {
    s32 sp20;

    return CCache__Request(0, CISet__GetROMAddressAndSize(arg1, arg3, arg2, &sp20), sp20, 0x1B, arg4, (void *)arg5, (char *)arg6, 1);
}
#else
void CCache__LoadPersistantSubBlock(s32 arg0, sptr arg1, s32 arg2, s32 arg3, sptr arg4, sptr arg5, sptr arg6) {
    s32 sp20;

    CCache__Request(0, CISet__GetROMAddressAndSize(arg1, arg3, arg2, &sp20), sp20, 0x1B, arg4, arg5, arg6, 1);
}
#endif
