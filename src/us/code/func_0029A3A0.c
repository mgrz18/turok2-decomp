/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_41.c, func_802B9FD0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int __osSpDeviceBusy();
extern s32 __osSpRawStartDma(s32 direction, s32 spAddress, s32 dramAddress, s32 size);
extern void __assert(void *arg0, void *arg1, s32 arg2);
extern s32 osVirtualToPhysical(s32);
s32 __osSpRawStartDma(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 & 7) {
        __assert("EX", "io/sprawdma.c", 0x3A);
    }
    if (arg2 & 7) {
        __assert("EX", "io/sprawdma.c", 0x3B);
    }
    if (arg3 & 7) {
        __assert("EX", "io/sprawdma.c", 0x3C);
    }
    if (__osSpDeviceBusy() != 0) {
        return -1;
    }
    (*(volatile u32 *)(0xA4040000U)) = arg1;
    (*(volatile u32 *)(0xA4040004U)) = osVirtualToPhysical(arg2);
    if (arg0 == 0) {
        (*(volatile u32 *)(0xA404000CU)) = arg3 - 1;
    } else {
        (*(volatile u32 *)(0xA4040008U)) = arg3 - 1;
    }
    return 0;
}
