#include "common.h"

s32 MtxPool_Reset(s32);
s32 guMtxIdent(s32);

typedef struct {
    /* 0x00 */ s32 items;
    /* 0x04 */ s32 halfCount;
} ItemList;

void MatrixHeap_Construct(ItemList *arg0, s32 arg1, s32 arg2) {
    s32 var_s0;
    s32 var_s1;

    var_s1 = 0;
    arg0->items = arg1;
    arg0->halfCount = (s32) (arg2 / 2);
    if (arg2 > 0) {
        var_s0 = arg1;
        do {
            guMtxIdent(var_s0);
            var_s1 += 1;
            var_s0 += 0x40;
        } while (var_s1 < arg2);
    }
    MtxPool_Reset((sptr) arg0);
}
