#include "common.h"
#include "m2c_macros.h"

extern u8 *D_800AF618[];

s32 func_00254564(void *arg0) {
    return *(s16 *)(D_800AF618[M2C_FIELD(arg0, s16 *, 0x996)] + 8) > 0;
}
