#include "common.h"

typedef struct PendingNode {
    /* 0x0 */ struct PendingNode *next;
    /* 0x4 */ s32 value;
} PendingNode;

/* Only the field written here: a list head. */
typedef struct {
    /* 0x000 */ u8 pad0[0x2E0];
    /* 0x2E0 */ PendingNode *pending;
} PendingOwner;

s32 osSetIntMask(s32);                              /* extern */

/* Pushes arg1 (holding arg2) on arg0's list with interrupts masked. */
void func_0028AF00(PendingOwner *arg0, PendingNode *arg1, s32 arg2) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    arg1->value = arg2;
    arg1->next = arg0->pending;
    arg0->pending = arg1;
    osSetIntMask(temp_v0);
}
