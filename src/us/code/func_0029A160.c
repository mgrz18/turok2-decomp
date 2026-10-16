/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_39.c, func_802B9D90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
typedef unsigned long long u64;
struct OSTask_t;
struct OSTask_t {
    u32 type;
    u32 flags;
    u64 *ucode_boot;
    u32 ucode_boot_size;
    u64 *ucode;
    u32 ucode_size;
    u64 *ucode_data;
    u32 ucode_data_size;
    u64 *dram_stack;
    u32 dram_stack_size;
    u64 *output_buff;
    u64 *output_buff_size;
    u64 *data_ptr;
    u32 data_size;
    u64 *yield_data_ptr;
    u32 yield_data_size;
};
struct OSTask_t;
typedef struct OSTask_t OSTask_t;
union OSTask;
union OSTask {
    OSTask_t t;
    long long force_structure_alignment;
};
union OSTask;
typedef union OSTask OSTask;
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
