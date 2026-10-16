/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_14.c, func_80254A28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct Pool;
struct Pool {
    s32 count;
    char pad[0x40];
    s32 field44;
};
struct Pool;
typedef struct Pool Pool;
struct ResourceCacheEntry;
struct ResourceCacheEntry {
    s32 resource;
    s32 size;
    s32 references;
    s32 flags;
    s32 lastUsed;
    s32 owner;
    struct ResourceCacheEntry *next;
    s32 key;
    s32 pending;
    s32 callback;
};
struct ResourceCacheEntry;
typedef struct ResourceCacheEntry ResourceCacheEntry;
extern void *func_00206250(s32 unused0, s32 arg1);
extern s32 CList__AddHead(void *arg0, s32 arg1);
void *func_00206250(s32 unused0, s32 arg1) {
    extern Pool D_800D8DAC;
    extern void **D_800D81D4;
    struct ResourceCacheEntry *temp_s0;
    if (D_800D8DAC.count == 0) {
        return 0;
    }
    D_800D8DAC.count -= 1;
    temp_s0 = D_800D81D4[D_800D8DAC.count];
    temp_s0->flags = 0x800;
    temp_s0->references = 0;
    temp_s0->resource = 0;
    temp_s0->callback = 0;
    temp_s0->pending = 0;
    temp_s0->owner = 0;
    temp_s0->lastUsed = D_800D8DAC.field44;
    CList__AddHead((char *)&D_800D8DAC - 0xBCC, (s32)temp_s0);
    temp_s0->flags |= (arg1 & 0xC);
    return temp_s0;
}
