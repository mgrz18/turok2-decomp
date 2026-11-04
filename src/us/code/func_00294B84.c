#include "common.h"

/* *D_800B7760: keeps a free list of nodes (each starts with its next pointer) at 0x2C. */
typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 *freeList;
} NodePool;

extern NodePool *D_800B7760;

/* Push onto the free list at 0x2C. */
void func_00294B84(void **arg0) {
    NodePool *owner = D_800B7760;

    *arg0 = owner->freeList;
    owner->freeList = (s32 *) arg0;
}
