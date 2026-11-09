#include "common.h"
#include "structs/cache.h"
#include "structs/decompressor.h"
#include "protos/ultra.h"


s32 CDecompressor__DecompressAsync(CDecompressor *thisx, CCacheEntry *entry, OSMesgQueue *replyQueue) {
    entry->decompressReplyQueue = replyQueue;
    return ~osSendMesg(&thisx->queue, entry, 0) != 0;
}
