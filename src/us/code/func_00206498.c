#include "common.h"
#include "structs/cache.h"

s32 CList__AddTail(s32, s32);
s32 CList__InsertBefore(s32, s32, s32);

extern CMemEntry *gCache_memLockedList;

void CCache__AddToAddressList(s32 arg0, CMemEntry *arg1) {
    CMemEntry *var_s0;

    var_s0 = gCache_memLockedList;
    if (var_s0 != NULL) {
loop_1:
        if (var_s0->data > arg1->data) {
            CList__InsertBefore((s32) &gCache_memLockedList, (s32) var_s0, (s32) arg1);
        } else {
            var_s0 = var_s0->lockedNext;
            if (var_s0 != NULL) {
                goto loop_1;
            }
        }
        if (var_s0 == NULL) {
            goto block_5;
        }
    } else {
block_5:
        CList__AddTail((s32) &gCache_memLockedList, (s32) arg1);
    }
    arg1->flags = arg1->flags | 0x1000;
}
