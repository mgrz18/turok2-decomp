#include "common.h"
#include "globals/engine.h"
#include "structs/decompressor.h"
#include "protos/ultra.h"

void *memset(void *dst, s32 value, uptr size);
void CDecompressor__Main(CDecompressor *thisx);

void CDecompressor__Construct(CDecompressor *thisx, OSId threadId) {
    osCreateMesgQueue(&thisx->queue, thisx->messages, DECOMPRESSOR_QUEUE_SIZE);
    memset(thisx->stack, threadId & 0xFF, DECOMPRESSOR_STACK_SIZE);
    osCreateThread(&thisx->thread, threadId, (void (*)(void *))CDecompressor__Main, thisx,
                   (u8 *)thisx->stack + DECOMPRESSOR_STACK_SIZE, gThreadPriorityDecompressorLow);
    osStartThread(&thisx->thread);
}

#include "structs/cache.h"

s32 CDecompressor__DecompressSync(CDecompressor *thisx, CCacheEntry *entry) {
    OSMesgQueue replyQueue;
    OSMesg replyMessage;
    OSMesg reply;

    osCreateMesgQueue(&replyQueue, &replyMessage, 1);
    entry->decompressReplyQueue = &replyQueue;
    gDecompressorBoostRetraces = 2;
    osSetThreadPri(&thisx->thread, gThreadPriorityDecompressorHigh);
    osJamMesg(&thisx->queue, entry, 1);
    osRecvMesg(&replyQueue, &reply, 1);
    return 1;
}

s32 CDecompressor__DecompressAsync(CDecompressor *thisx, CCacheEntry *entry, OSMesgQueue *replyQueue) {
    entry->decompressReplyQueue = replyQueue;
    return ~osSendMesg(&thisx->queue, entry, 0) != 0;
}

void CDecompressor__Flush(CDecompressor *thisx) {
    if (thisx->queue.validCount != 0) {
        do {
            osSetThreadPri(&thisx->thread, gThreadPriorityDecompressorHigh);
        } while (thisx->queue.validCount != 0);
    }
    osSetThreadPri(&thisx->thread, gThreadPriorityDecompressorLow);
}

/* CDecompressor__Main, after Turok 3's (Drahsid/turok3, src/us/decompressor.c),
 * which also calls func_241BD8() each round; Turok 2 does not. */


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
