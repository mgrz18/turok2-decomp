#include "common.h"
#include "globals/engine.h"
#include "structs/loader.h"
#include "protos/list.h"
#include "protos/ultra.h"

void *memset(void *dst, s32 value, u32 size);
void CLoader__Main(CLoader *thisx);

void CLoader__Construct(CLoader *thisx, OSId threadId) {
    CLoaderEntry *entry;
    s32 i;

    osCreateMesgQueue(&thisx->queue, thisx->messages, LOADER_QUEUE_SIZE);
    osCreateMesgQueue(&thisx->piReplyQueue, &thisx->piReplyMessage, 1);
    osSetEventMesg(8, &thisx->piReplyQueue, (OSMesg)LOADER_DMA_COMPLETE_MESSAGE); /* OS_EVENT_PI */
#ifdef TARGET_PORT
    CList__Construct(&thisx->freeList, T2_OFFSETOF(CLoaderEntry, prev), T2_OFFSETOF(CLoaderEntry, next));
#else
    CList__Construct(&thisx->freeList, 0x18, 0x1C);
#endif
#ifdef TARGET_PORT
    CList__Construct(&thisx->usedList, T2_OFFSETOF(CLoaderEntry, prev), T2_OFFSETOF(CLoaderEntry, next));
#else
    CList__Construct(&thisx->usedList, 0x18, 0x1C);
#endif
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
