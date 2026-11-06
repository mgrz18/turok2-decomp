#include "common.h"
#include "globals/engine.h"
#include "structs/decompressor.h"

void *memset(void *dst, s32 value, u32 size);
void osCreateMesgQueue(OSMesgQueue *queue, OSMesg *messages, s32 count);
void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void osStartThread(OSThread *thread);
void CDecompressor__Main(CDecompressor *thisx);

void CDecompressor__Construct(CDecompressor *thisx, OSId threadId) {
    osCreateMesgQueue(&thisx->queue, thisx->messages, DECOMPRESSOR_QUEUE_SIZE);
    memset(thisx->stack, threadId & 0xFF, DECOMPRESSOR_STACK_SIZE);
    osCreateThread(&thisx->thread, threadId, (void (*)(void *))CDecompressor__Main, thisx,
                   (u8 *)thisx->stack + DECOMPRESSOR_STACK_SIZE, gThreadPriorityDecompressorLow);
    osStartThread(&thisx->thread);
}
