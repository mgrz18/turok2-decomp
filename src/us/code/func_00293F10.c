#include "common.h"

typedef struct QueueNode {
    /* 0x00 */ struct QueueNode *next;
    /* 0x04 */ u8 pad4[0x10 - 0x4];
    /* 0x10 */ s32 stamp;
} QueueNode;

typedef struct {
    /* 0x00 */ QueueNode *head;
    /* 0x04 */ u8 pad4[0x20 - 0x4];
    /* 0x20 */ s32 stamp;
} Queue;

s32 osSetIntMask(s32);                              /* extern */

/* Pushes arg1 on arg0, stamped with arg0's current stamp, interrupts masked. */
void alSynAddPlayer(Queue *arg0, QueueNode *arg1) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    arg1->stamp = (s32) arg0->stamp;
    arg1->next = arg0->head;
    arg0->head = arg1;
    osSetIntMask(temp_v0);
}
