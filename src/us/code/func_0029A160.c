/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_39.c, func_802B9D90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern void *func_0029E460(void *destination, const void *source, int count);
extern OSTask *_VirtualToPhysicalTask(OSTask *arg0);
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
