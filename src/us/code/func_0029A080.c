/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_37.c, __osSiRawStartDma.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_0029A080(s32, s32);
extern int __osSiDeviceBusy();
extern void func_0029B020(void *arg0, void *arg1, s32 arg2);
extern int __osSiDeviceBusy(void);
extern void func_0029E340(s32 arg0, s32 arg1);
extern s32 func_0029BF80(s32);
extern void func_0029E030(s32 arg0, s32 arg1);
extern s32 D_800AA890;
extern s32 D_800AA894;
s32 func_0029A080(s32 arg0, s32 arg1) {
    if (arg1 & 3) {
        func_0029B020(&D_800AA890, &D_800AA894, 0x37);
    }
    if (__osSiDeviceBusy() != 0) {
        return -1;
    }
    if (arg0 == 1) {
        func_0029E340(arg1, 0x40);
    }
    (*(volatile u32 *)(0xA4800000U) = (func_0029BF80(arg1)));
    if (arg0 == 0) {
        (*(volatile u32 *)(0xA4800004U) = (0x1FC007C0U));
    } else {
        (*(volatile u32 *)(0xA4800010U) = (0x1FC007C0U));
    }
    if (arg0 == 0) {
        func_0029E030(arg1, 0x40);
    }
    return 0;
}
