#include "common.h"

/* Initialise a pool of 8 nodes: an empty used list and a free list holding them all. */

typedef struct {
    void *head; /* 0x00 */
    void *tail; /* 0x04 */
    u8 pad8[0xC];
} List; /* 0x14 bytes */

typedef struct {
    void *prev;  /* 0x00 */
    void *next;  /* 0x04 */
    s32 value;   /* 0x08 */
    f32 scale;   /* 0x0C */
    u8 pad10[6];
    s16 count;   /* 0x16 */
} PoolNode; /* 0x18 bytes */

typedef struct {
    List used;          /* 0x00 */
    List free;          /* 0x14 */
    PoolNode nodes[8];  /* 0x28 */
} Pool;

void CList__Construct(List *list, s32 prevOffset, s32 nextOffset);
s32 CList__AddTail(List *list, void *node);

void func_00466040(Pool *pool) {
    s32 i;
    PoolNode *node;

    CList__Construct(&pool->used, 0, 4);
    CList__Construct(&pool->free, 0, 4);
    for (i = 0; i < 8; i++) {
        node = &pool->nodes[i];
        CList__AddTail(&pool->free, node);
        node->value = 0;
        node->scale = 1.0f;
        node->count = 0;
    }
}
