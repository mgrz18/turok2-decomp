#include "common.h"

/* *D_800B7760: keeps a free list of nodes (each starts with its next pointer) at 0x2C. */
typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 *freeList;
} NodePool;

extern NodePool *D_800B7760;

s32 *func_00294B58(void) {
    s32 *temp_v0;
    s32 *var_v1;

    temp_v0 = D_800B7760->freeList;
    var_v1 = NULL;
    if (temp_v0 != NULL) {
        var_v1 = temp_v0;
        D_800B7760->freeList = (s32 *) *var_v1;
        *var_v1 = NULL;
    }
    return var_v1;
}
