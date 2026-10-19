/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_39.c, func_802B9E78_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int __osSpDeviceBusy();
extern void __osSpSetStatus(unsigned int arg0);
extern s32 func_0029A3A0(s32 direction, s32 spAddress, s32 dramAddress, s32 size);
extern OSTask *func_0029A160(OSTask *arg0);
extern void func_0029E340(OSTask *, s32);
extern void __osSpSetStatus(u32 arg0);
extern s32 func_0029A4A0(u32 arg0);
extern s32 __osSpDeviceBusy(void);
OSTask *func_0029A160(OSTask *intp);
void func_0029A248(OSTask *arg0) {
    OSTask *block;
    s32 result;
    block = func_0029A160(arg0);
    if (block->t.flags & 1) {
        block->t.ucode_data = block->t.yield_data_ptr;
        block->t.ucode_data_size = block->t.yield_data_size;
        arg0->t.flags &= ~1;
        if (block->t.flags & 4) {
            block->t.ucode = (u64 *)(*(volatile u32 *)(((u32)arg0->t.yield_data_ptr + 0xBFCU) | 0xA0000000U));
        }
    }
    func_0029E340(block, 0x40);
    __osSpSetStatus(0x2B00);
    do {
        result = func_0029A4A0(0x04001000);
    } while (result == -1);
    do {
        result = func_0029A3A0(1, 0x04000FC0, (s32)block, 0x40);
    } while (result == -1);
    while (__osSpDeviceBusy() != 0) {
    }
    do {
        result = func_0029A3A0(1, 0x04001000, (s32)block->t.ucode_boot, block->t.ucode_boot_size);
    } while (result == -1);
}
