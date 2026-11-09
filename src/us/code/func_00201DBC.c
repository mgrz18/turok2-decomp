/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de.c, func_8025637C_de.
 * The same code in Turok 2. */

#include "common.h"
#include "structs/loader.h"
#include "protos/ultra.h"

CLoaderEntry *CLoader__AllocLoaderEntry(CLoader *thisx);

#ifdef TARGET_PORT
#include "t2asset_game.h"

/* The host reads the whole request at once, on the caller's thread: the unit out of the
 * archive, RNC unpacked and in host byte order (docs/assets/PORT.md 10.4). */
void CLoader__LoadSync(CLoader *thisx, u32 address, s32 length, void *dest) {
    (void)thisx;
    T2Port_ReadAsset(address, dest, (u32)length);
}
#else
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
#endif
