/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_16.c, func_80255A34_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
struct HeapRecAtC;
typedef struct HeapRecAtC HeapRecAtC;
struct BlockRecAt20;
struct BlockRecAt20 {
    struct BlockRecAt20 *prevFree;
    struct BlockRecAt20 *nextFree;
    struct BlockRecAt20 *prev;
    struct BlockRecAt20 *next;
    u32 size;
    u32 free;
    u8 reserved18[8];
    u8 data[1];
};
struct BlockRecAt20;
struct HeapRecAtC;
struct HeapRecAtC {
    char pad0[8];
    struct BlockRecAt20 *head;
    struct BlockRecAt20 *tail;
};
struct BlockRecAt20;
typedef struct BlockRecAt20 BlockRecAt20;
void *func_002025F4(HeapRecAtC *heap, s32 request)
{
    BlockRecAt20 *block;
    BlockRecAt20 *split;
    u32 size;
    size = (request + 0x2F) & ~0xF;
    for (block = heap->tail; block != 0; block = block->prevFree) {
        if (block->free >= size) {
            { u8 *cursor = (u8 *)block; cursor += block->size; cursor += block->free; cursor -= size; split = (BlockRecAt20 *)cursor; }
            split->size = size;
            split->free = 0;
            block->free -= size;
            split->nextFree = 0;
            split->prevFree = 0;
            if (block->free == 0) {
                if (block->prevFree != 0) {
                    block->prevFree->nextFree = block->nextFree;
                }
                if (block->nextFree != 0) {
                    block->nextFree->prevFree = block->prevFree;
                }
                if (heap->head == block) {
                    heap->head = block->nextFree;
                }
                if (heap->tail == block) {
                    heap->tail = block->nextFree != 0 ? block->nextFree : block->prevFree;
                }
                block->prevFree = 0;
                block->nextFree = 0;
            }
            split->prev = block;
            split->next = block->next;
            if (block->next != 0) {
                split->next->prev = split;
            }
            block->next = split;
            return split->data;
        }
    }
    return 0;
}
