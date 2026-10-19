/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_39.c, func_802B9D90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern OSTask D_80121690;
extern void *func_0029E460(void *destination, const void *source, int count);
extern u32 func_0029BF80(void *addr);
extern OSTask *func_0029A160(OSTask *arg0);
OSTask *func_0029A160(OSTask *intp)
{
    OSTask *tp;
    OSTask *task = &D_80121690;
    tp = task;
    func_0029E460(task, intp, sizeof(OSTask));
    if (task->t.ucode != 0) { task->t.ucode = (void *)func_0029BF80(task->t.ucode); } (void)0;
    if (task->t.ucode_data != 0) { task->t.ucode_data = (void *)func_0029BF80(task->t.ucode_data); } (void)0;
    if (task->t.dram_stack != 0) { task->t.dram_stack = (void *)func_0029BF80(task->t.dram_stack); } (void)0;
    if (task->t.output_buff != 0) { task->t.output_buff = (void *)func_0029BF80(task->t.output_buff); } (void)0;
    if (task->t.output_buff_size != 0) { task->t.output_buff_size = (void *)func_0029BF80(task->t.output_buff_size); } (void)0;
    if (task->t.data_ptr != 0) { task->t.data_ptr = (void *)func_0029BF80(task->t.data_ptr); } (void)0;
    if (task->t.yield_data_ptr != 0) { task->t.yield_data_ptr = (void *)func_0029BF80(task->t.yield_data_ptr); } (void)0;
    return tp;
}
