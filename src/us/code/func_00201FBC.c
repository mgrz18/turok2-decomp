#include "common.h"
#include "structs/loader.h"

s32 osPiRawStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size);
s32 osRecvMesg(OSMesgQueue *queue, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *queue, OSMesg msg, s32 flags);
void osInvalDCache(void *addr, s32 size);
void osWritebackDCache(void *addr, s32 size);
void CLoader__DeallocLoaderEntry(CLoader *thisx, CLoaderEntry *entry);

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
