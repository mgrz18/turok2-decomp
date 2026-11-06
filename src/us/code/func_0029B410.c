/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_34.c, func_802B9B20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

extern s32 osPiRawReadIo(s32 arg0, s32 *arg1);
extern void __assert(void *arg0, void *arg1, s32 arg2);
s32 osPiRawReadIo(s32 arg0, s32 *arg1) {
    u32 status;
    if (arg1 == 0) {
        __assert("EX", "io/pirawread.c", 0x3D);
    }
    status = *(volatile u32 *)0xA4600010;
    while (status & 3) {
        status = *(volatile u32 *)0xA4600010;
    }
    *arg1 = *(s32 *)((u32) osRomBase | arg0 | 0xA0000000);
    return 0;
}
