#include "common.h"
#include "m2c_macros.h"

extern void *D_800B7760;

/* Push onto the free list at 0x2C. */
void func_00294B84(void **arg0) {
    void *owner = D_800B7760;

    *arg0 = M2C_FIELD(owner, void **, 0x2C);
    M2C_FIELD(owner, void **, 0x2C) = arg0;
}
