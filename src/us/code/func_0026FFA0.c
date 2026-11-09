/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_14.c, func_802570C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"
#include "protos/ultra.h"

extern CacheEntry *gAudioDmaCache;
extern void *func_0029E460(void *destination, const void *source, int count);
extern s32 alHeapDBAlloc(s32, s32, void *, s32, s32);
u32 AudioDMA_CacheBlock(void *block, s32 size) {
    extern u32 osVirtualToPhysical(void *);
    CacheEntry *entry;
    CacheEntry *unused;
    unused = 0;
    for (entry = gAudioDmaCache; entry != 0; entry = entry->next) {
        if (entry->key == block) {
            entry->state = 3;
            if (entry->size != size) {
                entry->size = size;
                func_0029E460(entry->data, block, size);
                osWritebackDCache(entry->data, size);
            }
            return osVirtualToPhysical(entry->data);
        }
        if (entry->state == 0) {
            unused = entry;
        }
    }
    if (unused == 0) {
        unused = (CacheEntry *)alHeapDBAlloc(0, 0, gAudioHeap, 1, 0x90);
        unused->next = gAudioDmaCache;
        gAudioDmaCache = unused;
        sAudioDMACacheBlocks++;
    }
    unused->key = block;
    unused->state = 3;
    unused->size = size;
    func_0029E460(unused->data, block, size);
    return osVirtualToPhysical(unused->data);
}
