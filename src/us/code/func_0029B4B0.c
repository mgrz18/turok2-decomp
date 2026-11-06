/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_14.c, func_802BB060_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int __osSiRawReadIo(unsigned int devAddr, unsigned int * data);
extern int __osSiDeviceBusy();
extern void __assert(void *arg0, void *arg1, s32 arg2);
int __osSiRawReadIo(u32 arg0, u32 *arg1) {
    if (arg0 & 3) {
        __assert("EX", "io/sirawread.c", 0x33);
    }
    if (arg1 == 0) {
        __assert("EX", "io/sirawread.c", 0x34);
    }
    if (__osSiDeviceBusy() != 0) {
        return -1;
    }
    *arg1 = *(u32 *)(arg0 | 0xA0000000);
    return 0;
}
extern void __assert(void *arg0, void *arg1, s32 arg2);
