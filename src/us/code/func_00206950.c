#include "common.h"
#include "structs/world.h"

/* Takes arg1 0x40-byte blocks off the matrix heap (MtxPool, structs/world.h); 0 if there are
 * not enough. */
u8 *MtxPool_Alloc(MtxPool *arg0, s32 arg1) {
    u8 *var_a2;

    var_a2 = 0;
    if (arg0->remaining >= arg1) {
        var_a2 = arg0->next;
        arg0->next = var_a2 + (arg1 << 6);
        arg0->remaining = (s32) (arg0->remaining - arg1);
    }
    return var_a2;
}
