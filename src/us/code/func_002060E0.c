#include "common.h"
#include "structs/cache.h"
#include "protos/cache.h"


CMemEntry *CCache__AllocateInternal(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v0_2;
    CMemEntry *var_s0;

    var_s0 = CCache__AllocMemEntry(0, arg2);
    if (var_s0 != 0) {
        var_s0->lockCount += 1;
        var_s0->flags |= 0x100;
        temp_v0 = CCache__HeapAlloc(0, arg1, ((u32) arg2 >> 5) & 1, arg2);
        var_s0->data = (void *)temp_v0;
        if (temp_v0 != 0) {
            var_s0->size = arg1;
            var_s0->flags |= arg2;
            CCache__AddToAddressList(0, var_s0);
            return var_s0;
        }
        temp_v0_2 = var_s0->lockCount - 1;
        var_s0->lockCount = temp_v0_2;
        if (temp_v0_2 == 0) {
            var_s0->flags &= ~0x100;
        }
        CCache__FreeMemEntry(0, var_s0);
        var_s0 = NULL;
        goto block_6;
    }
block_6:
    return var_s0;
}
