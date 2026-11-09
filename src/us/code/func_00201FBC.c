#include "common.h"
#include "structs/loader.h"
#include "protos/ultra.h"

s32 osPiRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size);
void CLoader__DeallocLoaderEntry(CLoader *thisx, CLoaderEntry *entry);

#ifdef TARGET_PORT
#include "t2asset_game.h"

/* The loader thread on the host: no PI DMA and no cache maintenance; the request is read
 * whole through the asset reader (docs/assets/PORT.md 10.4), then replied to as on the N64. */
void CLoader__Main(CLoader *thisx) {
    CLoaderEntry *entry;

    for (;;) {
        osRecvMesg(&thisx->queue, (OSMesg *)&entry, 1);
        T2Port_ReadAsset(entry->address, entry->dest, (u32)entry->length);
        if (entry->replyQueue != NULL) {
            osSendMesg(entry->replyQueue, entry->replyMessage, 1);
        }
        CLoader__DeallocLoaderEntry(thisx, entry);
    }
}
#else

/* The loader thread: takes requests off queue, DMAs them in (OS_READ) and
 * waits for the PI on piReplyQueue, then replies and frees the entry. */
void CLoader__Main(CLoader *thisx) {
    CLoaderEntry *entry;
    OSMesg message;

loop:
    {
        osRecvMesg(&thisx->queue, (OSMesg *)&entry, 1);
        osWritebackDCache(entry->dest, entry->length);
        osPiRawStartDma(0, entry->address, entry->dest, entry->length);
        osRecvMesg(&thisx->piReplyQueue, &message, 1);
        osInvalDCache(entry->dest, entry->length);
        if (entry->replyQueue != NULL) {
            osSendMesg(entry->replyQueue, entry->replyMessage, 1);
        }
        CLoader__DeallocLoaderEntry(thisx, entry);
    }
    goto loop;
}
#endif
