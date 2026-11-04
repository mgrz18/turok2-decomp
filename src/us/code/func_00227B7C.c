#include "common.h"

/* An instance in a pool: flag bit 0 = allocated, a counter to decrement
 * on release, and prev/next links. */
typedef struct PoolItem {
    /* 0x000 */ u8 pad0[0x1A0];
    /* 0x1A0 */ s32 flags;
    /* 0x1A4 */ s32 *counter;
    /* 0x1A8 */ struct PoolItem *prev;
    /* 0x1AC */ struct PoolItem *next;
} PoolItem;

typedef struct {
    /* 0x0000 */ u8 pad0[0x3600];
    /* 0x3600 */ PoolItem *freeHead;
    /* 0x3604 */ PoolItem *usedHead;
    /* 0x3608 */ PoolItem *usedTail;
} Pool00227B7C;

/* Releases arg1 back to the pool's free list. */
void func_00227B7C(Pool00227B7C *arg0, PoolItem *arg1) {
    s32 *temp_v1;
    s32 temp_a2;
    PoolItem *temp_v1_2;
    PoolItem *temp_v1_3;

    temp_a2 = arg1->flags;
    if (temp_a2 & 1) {
        temp_v1 = arg1->counter;
        arg1->flags = temp_a2 & ~1;
        if (temp_v1 != NULL) {
            *temp_v1 -= 1;
        }
        temp_v1_2 = arg1->prev;
        if (temp_v1_2 != NULL) {
            temp_v1_2->next = arg1->next;
        }
        temp_v1_3 = arg1->next;
        if (temp_v1_3 != NULL) {
            temp_v1_3->prev = arg1->prev;
        }
        if (arg0->usedHead == arg1) {
            arg0->usedHead = arg1->next;
        }
        if (arg0->usedTail == arg1) {
            arg0->usedTail = arg1->prev;
        }
        arg1->next = arg0->freeHead;
        arg1->prev = NULL;
        arg0->freeHead = arg1;
    }
}
