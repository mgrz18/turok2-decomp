/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BCC90_de_7.c, func_802B7400_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/ultra.h"

extern s32 __osAiDeviceBusy();
extern u8 hdwrBugFlag[]; /* local type: an array, not the u8 of globals/libultra.h; as a scalar GCC 2 orders the code differently */
s32 osAiSetNextBuffer(void *bufPtr, u32 size) {
    char *bptr = bufPtr;
    if (hdwrBugFlag[0] != 0) {
        bptr -= 0x2000;
    }
    if ((((u32)bufPtr + size) & 0x1FFF) == 0) {
        hdwrBugFlag[0] = 1;
    } else {
        hdwrBugFlag[0] = 0;
    }
    if (__osAiDeviceBusy() != 0) {
        return -1;
    }
    *(volatile u32 *)0xA4500000 = osVirtualToPhysical(bptr);
    *(volatile u32 *)0xA4500004 = size;
    return 0;
}
