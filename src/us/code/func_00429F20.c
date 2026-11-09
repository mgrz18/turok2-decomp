#include "common.h"
#include "structs/world.h"

void MtxPool_Reset(MtxPool *);
void guMtxIdent(void *);

/* Sets every block of the pool (arg2 blocks at arg1) to identity; two halves of arg2 / 2. */
void MatrixHeap_Construct(MtxPool *arg0, MtxBlock *arg1, s32 arg2) {
    MtxBlock *var_s0;
    s32 var_s1;

    var_s1 = 0;
    arg0->base = arg1;
    arg0->n = (s32) (arg2 / 2);
    if (arg2 > 0) {
        var_s0 = arg1;
        do {
            guMtxIdent(var_s0);
            var_s1 += 1;
            var_s0 += 1;
        } while (var_s1 < arg2);
    }
    MtxPool_Reset(arg0);
}
