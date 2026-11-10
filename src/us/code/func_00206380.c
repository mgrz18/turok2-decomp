#include "common.h"
#include "m2c_macros.h"
#include "protos/cache.h"


#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 CCache__FreeOldestMemEntryWithFlags(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_v0;

    var_v0 = arg1;
    if (arg2 & 0x10) {
        var_v0 = 0;
    }
    return CCache__FreeOldestMemEntry(0, 2, var_v0);
}
#else
void CCache__FreeOldestMemEntryWithFlags(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_v0;

    var_v0 = arg1;
    if (arg2 & 0x10) {
        var_v0 = 0;
    }
    CCache__FreeOldestMemEntry(0, 2, var_v0);
}
#endif
