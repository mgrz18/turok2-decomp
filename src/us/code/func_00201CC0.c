#include "common.h"
#include "globals/engine.h"
#include "structs/loader.h"

void CList__Construct(CList *list, s32 lastOffset, s32 nextOffset);
void CList__AddTail(CList *list, void *element);
void *memset(void *dst, s32 value, u32 size);
void osCreateMesgQueue(OSMesgQueue *queue, OSMesg *messages, s32 count);
void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void osSetEventMesg(s32 event, OSMesgQueue *queue, OSMesg msg);
void osStartThread(OSThread *thread);
void CLoader__Main(CLoader *thisx);

void CLoader__Construct(CLoader *thisx, OSId threadId) {
    CLoaderEntry *entry;
    s32 i;

    osCreateMesgQueue(&thisx->queue, thisx->messages, LOADER_QUEUE_SIZE);
    osCreateMesgQueue(&thisx->piReplyQueue, &thisx->piReplyMessage, 1);
    osSetEventMesg(8, &thisx->piReplyQueue, (OSMesg)LOADER_DMA_COMPLETE_MESSAGE); /* OS_EVENT_PI */
    CList__Construct(&thisx->freeList, 0x18, 0x1C);
    CList__Construct(&thisx->usedList, 0x18, 0x1C);
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
