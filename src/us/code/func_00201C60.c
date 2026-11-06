#include "common.h"
#include "structs/cache.h"
#include "globals/engine.h"
#include "structs/decompressor.h"

/* CDecompressor__Main, after Turok 3's (Drahsid/turok3, src/us/decompressor.c),
 * which also calls func_241BD8() each round; Turok 2 does not. */

s32 osRecvMesg(void *queue, void *msg, s32 flags);
s32 osSendMesg(void *queue, void *msg, s32 flags);

/* The decompressor thread: takes a cache entry from its queue, runs the
 * entry's callback, and sends the entry back to whoever asked. */
void CDecompressor__Main(CDecompressor *thisx) {
    CCacheEntry *entry;

    while (1) {
        osRecvMesg(&thisx->queue, &entry, 1);
        thisx->lastDecompression = gFrameCount;
        entry->decompressCallback(entry->notifyId, entry);
        osSendMesg(entry->decompressReplyQueue, entry, 1);
    }
}
