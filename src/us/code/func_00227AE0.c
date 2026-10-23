#include "common.h"

typedef struct Node {
    u8 pad0[0x18];
    s32 unk18[7];         /* 0x18 */
    u8 pad34[0x198 - 0x34];
    s32 unk198;           /* 0x198 */
    u8 pad19C[4];
    s32 flags;            /* 0x1A0 */
    u8 pad1A4[4];
    struct Node *prev;    /* 0x1A8 */
    struct Node *next;    /* 0x1AC */
} Node;

typedef struct Pool {
    u8 pad0[0x3600];
    Node *freeList;       /* 0x3600 */
    Node *head;           /* 0x3604 */
    Node *tail;           /* 0x3608 */
} Pool;

extern void func_00227B7C(Pool *pool, Node *node);

Node *func_00227AE0(Pool *pool) {
    Node *node;

    if (pool->freeList == NULL) {
        func_00227B7C(pool, pool->tail);
    }
    node = pool->freeList;
    pool->freeList = node->next;
    if (pool->head != NULL) {
        pool->head->prev = node;
    }
    node->next = pool->head;
    node->prev = NULL;
    pool->head = node;
    if (pool->tail == NULL) {
        pool->tail = node;
    }
    node->unk18[0] = 0;
    node->unk18[1] = 0;
    node->unk18[2] = 0;
    node->unk18[3] = 0;
    node->unk18[4] = 0;
    node->unk18[5] = 0;
    node->unk18[6] = 0;
    node->unk198 = 0;
    node->flags |= 1;
    return node;
}
