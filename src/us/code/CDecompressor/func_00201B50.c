#include "common.h"
#include "globals/engine.h"
#include "structs/cache.h"
#include "structs/decompressor.h"
#include "protos/ultra.h"


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
