#include "common.h"

/* A node: linked at 0, a self pointer at 0x14 and three bytes at 0x31. */
typedef struct PoolNode {
    /* 0x00 */ struct PoolNode *next;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ struct PoolNode *self;
    /* 0x18 */ u8 pad18[0x31 - 0x18];
    /* 0x31 */ s8 unk31;
    /* 0x32 */ s8 unk32;
    /* 0x33 */ s8 unk33;
} PoolNode;

/* The pool: the used list (head 0x64, tail 0x68) and the free list (0x6C). */
typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ PoolNode *usedHead;
    /* 0x68 */ PoolNode *usedTail;
    /* 0x6C */ PoolNode *freeList;
} NodeQueue;

/* Takes a node off the free list and appends it to the used list. */
PoolNode *__mapVoice(NodeQueue *arg0, s8 arg1, s8 arg2, s8 arg3) {
    PoolNode *temp_v1;

    temp_v1 = arg0->freeList;
    if (temp_v1 != NULL) {
        arg0->freeList = temp_v1->next;
        temp_v1->next = NULL;
        if (arg0->usedHead == NULL) {
            arg0->usedHead = temp_v1;
        } else {
            arg0->usedTail->next = temp_v1;
        }
        arg0->usedTail = temp_v1;
        temp_v1->unk31 = arg3;
        temp_v1->unk32 = arg1;
        temp_v1->unk33 = arg2;
        temp_v1->self = temp_v1;
    }
    return temp_v1;
}
