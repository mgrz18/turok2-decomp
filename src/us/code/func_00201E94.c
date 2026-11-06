/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de.c, func_80256454_de.
 * The same code in Turok 2. */

#include "common.h"
#include "structs/loader.h"

s32 osJamMesg(OSMesgQueue *queue, OSMesg msg, s32 flags);
s32 osSendMesg(OSMesgQueue *queue, OSMesg msg, s32 flags);
CLoaderEntry *CLoader__AllocLoaderEntry(CLoader *thisx);

/* Queues a read of length bytes of ROM at address into dest; the last
 * entry sends replyMessage to replyQueue. A direct read is one entry,
 * jammed at the front of the queue. */
void CLoader__LoadAsync(CLoader *thisx, u32 address, s32 length, void *dest,
                        OSMesgQueue *replyQueue, OSMesg replyMessage, s32 direct) {
    s32 remaining;
    s32 chunkLength;
    CLoaderEntry *entry;

    if (direct != 0) {
        entry = CLoader__AllocLoaderEntry(thisx);
        if (entry != NULL) {
            entry->address = address;
            entry->dest = dest;
            entry->length = length;
            entry->replyQueue = replyQueue;
            entry->replyMessage = replyMessage;
            osJamMesg(&thisx->queue, entry, 1);
        }
    } else {
        remaining = length;
        do {
            entry = CLoader__AllocLoaderEntry(thisx);
            chunkLength = LOADER_CHUNK_SIZE;
            if (entry == NULL) {
                break;
            }
            entry->address = address + (length - remaining);
            entry->dest = (u8 *)dest + (length - remaining);
            if ((u32)remaining < LOADER_CHUNK_SIZE + 1) {
                chunkLength = remaining;
            }
            remaining -= chunkLength;
            entry->length = chunkLength;
            if (remaining != 0) {
                entry->replyQueue = NULL;
                entry->replyMessage = NULL;
            } else {
                entry->replyQueue = replyQueue;
                entry->replyMessage = replyMessage;
            }
            osSendMesg(&thisx->queue, entry, 1);
        } while (entry != NULL && remaining != 0);
    }
}
