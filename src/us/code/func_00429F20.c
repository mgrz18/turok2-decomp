#include "common.h"
#include "structs/world.h"

void MtxPool_Reset(MtxPool *);
s32 guMtxIdent(sptr);

/* Fills the matrix heap with arg2 identity matrices at arg1 (two halves of arg2 / 2). */
void MatrixHeap_Construct(MtxPool *arg0, sptr arg1, s32 arg2) {
    sptr var_s0;
    s32 var_s1;

    var_s1 = 0;
    arg0->base = (u8 *) arg1;
    arg0->n = (s32) (arg2 / 2);
    if (arg2 > 0) {
        var_s0 = arg1;
        do {
            guMtxIdent(var_s0);
            var_s1 += 1;
            var_s0 += 0x40;
        } while (var_s1 < arg2);
    }
    MtxPool_Reset(arg0);
}
