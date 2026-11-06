#include "common.h"
#include "structs/cache.h"
#include "structs/decompressor.h"

s32 osSendMesg(OSMesgQueue *queue, OSMesg msg, s32 flags);

s32 CDecompressor__DecompressAsync(CDecompressor *thisx, CCacheEntry *entry, OSMesgQueue *replyQueue) {
    entry->decompressReplyQueue = replyQueue;
    return ~osSendMesg(&thisx->queue, entry, 0) != 0;
}
