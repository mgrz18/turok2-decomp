#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 headerSize;
    /* 0x14 */ s32 size;
} HeapBlock;

typedef struct {
    /* 0x00 */ u8 *start;
    /* 0x04 */ u8 *end;
    /* 0x08 */ HeapBlock *free8;
    /* 0x0C */ HeapBlock *freeC;
    /* 0x10 */ HeapBlock *first;
} Heap;

void func_00202474(Heap *heap, u8 *start, s32 size) {
    u8 *end = start + size;
    HeapBlock *block = (HeapBlock *)(((u32)start + 15) & ~15);

    size -= (u8 *)block - start;
    size &= ~15;
    heap->start = start;
    heap->end = end;
    block->unk4 = 0;
    block->unk0 = 0;
    block->unkC = 0;
    block->unk8 = 0;
    block->headerSize = 0x20;
    block->size = size - 0x20;
    heap->first = block;
    if (block->size != 0) {
        heap->freeC = block;
        heap->free8 = block;
    } else {
        heap->freeC = NULL;
        heap->free8 = NULL;
    }
}
