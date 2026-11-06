#include "common.h"

typedef struct Block0020280C {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Block0020280C *next;
    /* 0x08 */ u8 pad8[0x14 - 0x8];
    /* 0x14 */ u32 size;
} Block0020280C;

typedef struct {
    /* 0x0 */ u8 pad0[8];
    /* 0x8 */ Block0020280C *head;
} Heap0020280C;

/* Sums the block sizes into *arg1 and keeps the largest in *arg2. */
void CHeap__GetFreeStats(Heap0020280C *arg0, s32 *arg1, u32 *arg2) {
    u32 temp_a0;
    u32 var_v1;
    Block0020280C *var_a3;

    *arg1 = 0;
    *arg2 = 0;
    var_a3 = arg0->head;
    if (var_a3 != NULL) {
        do {
            *arg1 += var_a3->size;
            var_v1 = var_a3->size;
            temp_a0 = *arg2;
            if (var_v1 < temp_a0) {
                var_v1 = temp_a0;
            }
            *arg2 = var_v1;
            var_a3 = var_a3->next;
        } while (var_a3 != NULL);
    }
}
