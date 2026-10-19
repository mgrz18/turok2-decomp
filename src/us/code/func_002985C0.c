/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BCC90_de_7.c, func_802B7400_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 __osAiDeviceBusy();
extern u8 D_800B7A20[];
extern u32 func_0029BF80(void *);
s32 func_002985C0(void *bufPtr, u32 size) {
    char *bptr = bufPtr;
    if (D_800B7A20[0] != 0) {
        bptr -= 0x2000;
    }
    if ((((u32)bufPtr + size) & 0x1FFF) == 0) {
        D_800B7A20[0] = 1;
    } else {
        D_800B7A20[0] = 0;
    }
    if (__osAiDeviceBusy() != 0) {
        return -1;
    }
    *(volatile u32 *)0xA4500000 = func_0029BF80(bptr);
    *(volatile u32 *)0xA4500004 = size;
    return 0;
}
