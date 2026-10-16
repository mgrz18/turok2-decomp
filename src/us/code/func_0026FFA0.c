/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_14.c, func_802570C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern int D_800B2674;
struct CacheEntry;
struct CacheEntry {
    struct CacheEntry *next;
    void *key;
    u16 state;
    u16 size;
    s32 padC;
    char data[0x80];
};
struct CacheEntry;
typedef struct CacheEntry CacheEntry;
extern CacheEntry *D_800F279C;
extern void *D_800F2840;
extern void *func_0029E460(void *destination, const void *source, int count);
extern void func_0029E340(void *, s32);
extern s32 func_002912A0(s32, s32, void *, s32, s32);
u32 func_0026FFA0(void *block, s32 size) {
    extern u32 func_0029BF80(void *);
    CacheEntry *entry;
    CacheEntry *unused;
    unused = 0;
    for (entry = D_800F279C; entry != 0; entry = entry->next) {
        if (entry->key == block) {
            entry->state = 3;
            if (entry->size != size) {
                entry->size = size;
                func_0029E460(entry->data, block, size);
                func_0029E340(entry->data, size);
            }
            return func_0029BF80(entry->data);
        }
        if (entry->state == 0) {
            unused = entry;
        }
    }
    if (unused == 0) {
        unused = (CacheEntry *)func_002912A0(0, 0, D_800F2840, 1, 0x90);
        unused->next = D_800F279C;
        D_800F279C = unused;
        D_800B2674++;
    }
    unused->key = block;
    unused->state = 3;
    unused->size = size;
    func_0029E460(unused->data, block, size);
    return func_0029BF80(unused->data);
}
