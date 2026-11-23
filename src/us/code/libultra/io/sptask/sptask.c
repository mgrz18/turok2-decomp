/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_39.c, func_802B9D90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern void *func_0029E460(void *destination, const void *source, int count);
OSTask *_VirtualToPhysicalTask(OSTask *intp);

OSTask *_VirtualToPhysicalTask(OSTask *intp)
{
    OSTask *tp;
    OSTask *task = &__osTmpTask;
    tp = task;
    func_0029E460(task, intp, sizeof(OSTask));
    if (task->t.ucode != 0) { task->t.ucode = (void *)osVirtualToPhysical(task->t.ucode); } (void)0;
    if (task->t.ucode_data != 0) { task->t.ucode_data = (void *)osVirtualToPhysical(task->t.ucode_data); } (void)0;
    if (task->t.dram_stack != 0) { task->t.dram_stack = (void *)osVirtualToPhysical(task->t.dram_stack); } (void)0;
    if (task->t.output_buff != 0) { task->t.output_buff = (void *)osVirtualToPhysical(task->t.output_buff); } (void)0;
    if (task->t.output_buff_size != 0) { task->t.output_buff_size = (void *)osVirtualToPhysical(task->t.output_buff_size); } (void)0;
    if (task->t.data_ptr != 0) { task->t.data_ptr = (void *)osVirtualToPhysical(task->t.data_ptr); } (void)0;
    if (task->t.yield_data_ptr != 0) { task->t.yield_data_ptr = (void *)osVirtualToPhysical(task->t.yield_data_ptr); } (void)0;
    return tp;
}

extern int __osSpDeviceBusy();
extern void __osSpSetStatus(unsigned int arg0);
extern s32 __osSpRawStartDma(s32 direction, s32 spAddress, sptr dramAddress, s32 size);
extern void __osSpSetStatus(u32 arg0);
extern s32 __osSpSetPc(u32 arg0);
extern s32 __osSpDeviceBusy(void);

void osSpTaskLoad(OSTask *arg0) {
    OSTask *block;
    sptr result;
    block = _VirtualToPhysicalTask(arg0);
    if (block->t.flags & 1) {
        block->t.ucode_data = block->t.yield_data_ptr;
        block->t.ucode_data_size = block->t.yield_data_size;
        arg0->t.flags &= ~1;
        if (block->t.flags & 4) {
            block->t.ucode = (u64 *)(*(volatile u32 *)(((uptr)arg0->t.yield_data_ptr + 0xBFCU) | 0xA0000000U));
        }
    }
    osWritebackDCache(block, 0x40);
    __osSpSetStatus(0x2B00);
    do {
        result = __osSpSetPc(0x04001000);
    } while (result == -1);
    do {
        result = __osSpRawStartDma(1, 0x04000FC0, (sptr)block, 0x40);
    } while (result == -1);
    while (__osSpDeviceBusy() != 0) {
    }
    do {
        result = __osSpRawStartDma(1, 0x04001000, (sptr)block->t.ucode_boot, block->t.ucode_boot_size);
    } while (result == -1);
}

#include "common.h"

s32 __osSpDeviceBusy();                                /* extern */

void osSpTaskStartGo(OSTask *tp) {
    do {

    } while (__osSpDeviceBusy() != 0);
    __osSpSetStatus(0x125);
}
