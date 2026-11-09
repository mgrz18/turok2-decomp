#include "common.h"
#include "structs/world.h"

/* Takes arg1 blocks off the pool; NULL if there are not enough. */
MtxBlock *MtxPool_Alloc(MtxPool *arg0, s32 arg1) {
    MtxBlock *var_a2;

    var_a2 = 0;
    if (arg0->remaining >= arg1) {
        var_a2 = arg0->next;
        arg0->next = (MtxBlock *) ((u8 *) var_a2 + (arg1 << 6));
        arg0->remaining = (s32) (arg0->remaining - arg1);
    }
    return var_a2;
}
