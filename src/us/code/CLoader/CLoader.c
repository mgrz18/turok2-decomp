#include "common.h"
#include "globals/engine.h"
#include "structs/loader.h"
#include "protos/list.h"
#include "protos/ultra.h"

void *memset(void *dst, s32 value, uptr size);
void CLoader__Main(CLoader *thisx);

void CLoader__Construct(CLoader *thisx, OSId threadId) {
    CLoaderEntry *entry;
    s32 i;

    osCreateMesgQueue(&thisx->queue, thisx->messages, LOADER_QUEUE_SIZE);
    osCreateMesgQueue(&thisx->piReplyQueue, &thisx->piReplyMessage, 1);
    osSetEventMesg(8, &thisx->piReplyQueue, (OSMesg)LOADER_DMA_COMPLETE_MESSAGE); /* OS_EVENT_PI */
    CList__Construct(&thisx->freeList, T2_OFFSETOF(CLoaderEntry, prev), T2_OFFSETOF(CLoaderEntry, next));
    CList__Construct(&thisx->usedList, T2_OFFSETOF(CLoaderEntry, prev), T2_OFFSETOF(CLoaderEntry, next));
    i = 0;
    do {
        entry = &thisx->entries[i];
        entry->flags = LOADERFLAGS_UNALLOCATED;
        CList__AddTail(&thisx->freeList, entry);
        i++;
    } while (i < LOADER_ENTRY_COUNT);
    memset(thisx->stack, threadId & 0xFF, LOADER_STACK_SIZE);
    osCreateThread(&thisx->thread, threadId, (void (*)(void *))CLoader__Main, thisx,
                   (u8 *)thisx->stack + LOADER_STACK_SIZE, gThreadPriorityLoader);
    osStartThread(&thisx->thread);
}
CLoaderEntry *CLoader__AllocLoaderEntry(CLoader *thisx);
#ifdef TARGET_PORT
#include "t2asset_game.h"

/* The host reads the whole request at once, on the caller's thread: the unit out of the
 * archive, RNC unpacked and in host byte order (docs/assets/PORT.md 10.4). */
void CLoader__LoadSync(CLoader *thisx, uptr address, sptr length, void *dest) {
    (void)thisx;
    T2Port_ReadAsset(address, dest, (u32)length);
}
#else
/* Reads length bytes of ROM at address into dest, LOADER_CHUNK_SIZE at a
 * time, waiting for each. */
void CLoader__LoadSync(CLoader *thisx, uptr address, sptr length, void *dest) {
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

/* Queues a read of length bytes of ROM at address into dest; the last
 * entry sends replyMessage to replyQueue. A direct read is one entry,
 * jammed at the front of the queue. */
void CLoader__LoadAsync(CLoader *thisx, u32 address, s32 length, void *dest,
                        OSMesgQueue *replyQueue, OSMesg replyMessage, s32 direct) {
#ifdef TARGET_PORT
    /* One entry per request on the host: chunks at address + k * 0x2000 would name addresses
     * that are not unit keys (and, for an unpacked RNC unit, not even the same bytes). The
     * queue, the reply and the direct jam stay (docs/assets/PORT.md 10.4). */
    CLoaderEntry *entry = CLoader__AllocLoaderEntry(thisx);
    if (entry == NULL) {
        return;
    }
    entry->address = address;
    entry->dest = dest;
    entry->length = length;
    entry->replyQueue = replyQueue;
    entry->replyMessage = replyMessage;
    if (direct != 0) {
        osJamMesg(&thisx->queue, entry, 1);
    } else {
        osSendMesg(&thisx->queue, entry, 1);
    }
#else
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
#endif
}

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

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);

CLoaderEntry *CLoader__AllocLoaderEntry(CLoader *thisx) {
    u32 mask;
    CLoaderEntry *entry;

    mask = __osDisableInt();
    entry = thisx->freeList.m_pHead;
    if (entry != NULL) {
        CList__Remove(&thisx->freeList, entry);
        entry->flags = LOADERFLAGS_ALLOCATED;
        CList__AddHead(&thisx->usedList, entry);
    }
    __osRestoreInt(mask);
    return entry;
}

/* Turok 3's loader.c CLoader__DeallocLoaderEntry: the used list back to the
 * free list, with interrupts off (CLoader__AllocLoaderEntry's mirror). */
void CLoader__DeallocLoaderEntry(CLoader *thisx, CLoaderEntry *entry) {
    u32 mask = __osDisableInt();

    CList__Remove(&thisx->usedList, entry);
    entry->flags = LOADERFLAGS_UNALLOCATED;
    CList__AddHead(&thisx->freeList, entry);
    __osRestoreInt(mask);
}
