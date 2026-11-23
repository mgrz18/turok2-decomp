#include "common.h"
#include "structs/heap.h"

/* Sums the block sizes into *arg1 and keeps the largest in *arg2. */
void CHeap__GetFreeStats(CHeap *heap, s32 *arg1, u32 *arg2) {
    u32 temp_a0;
    u32 var_v1;
    CHeapBlock *var_a3;

    *arg1 = 0;
    *arg2 = 0;
    var_a3 = heap->freeHead;
    if (var_a3 != NULL) {
        do {
            *arg1 += var_a3->free;
            var_v1 = var_a3->free;
            temp_a0 = *arg2;
            if (var_v1 < temp_a0) {
                var_v1 = temp_a0;
            }
            *arg2 = var_v1;
            var_a3 = var_a3->freeNext;
        } while (var_a3 != NULL);
    }
}
