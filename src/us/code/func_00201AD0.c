#include "common.h"
#include "globals/engine.h"
#include "structs/decompressor.h"
#include "protos/ultra.h"

void *memset(void *dst, s32 value, u32 size);
void CDecompressor__Main(CDecompressor *thisx);

void CDecompressor__Construct(CDecompressor *thisx, OSId threadId) {
    osCreateMesgQueue(&thisx->queue, thisx->messages, DECOMPRESSOR_QUEUE_SIZE);
    memset(thisx->stack, threadId & 0xFF, DECOMPRESSOR_STACK_SIZE);
    osCreateThread(&thisx->thread, threadId, (void (*)(void *))CDecompressor__Main, thisx,
                   (u8 *)thisx->stack + DECOMPRESSOR_STACK_SIZE, gThreadPriorityDecompressorLow);
    osStartThread(&thisx->thread);
}
