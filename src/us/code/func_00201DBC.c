/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de.c, func_8025637C_de.
 * The same code in Turok 2. */

#include "common.h"
#include "structs/loader.h"

s32 osRecvMesg(OSMesgQueue *queue, OSMesg *msg, s32 flags);
s32 osJamMesg(OSMesgQueue *queue, OSMesg msg, s32 flags);
void osCreateMesgQueue(OSMesgQueue *queue, OSMesg *messages, s32 count);
CLoaderEntry *CLoader__AllocLoaderEntry(CLoader *thisx);

/* Reads length bytes of ROM at address into dest, LOADER_CHUNK_SIZE at a
 * time, waiting for each. */
void CLoader__LoadSync(CLoader *thisx, u32 address, s32 length, void *dest) {
    OSMesgQueue replyQueue;
    OSMesg replyMessage;
    OSMesg reply;
    s32 remaining;
    s32 chunkLength;
    CLoaderEntry *entry;

    osCreateMesgQueue(&replyQueue, &replyMessage, 1);
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
        entry->length = chunkLength;
        entry->replyQueue = &replyQueue;
        osJamMesg(&thisx->queue, entry, 1);
        osRecvMesg(&replyQueue, &reply, 1);
        remaining -= chunkLength;
    } while (remaining != 0);
}
