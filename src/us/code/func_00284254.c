#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00410CE8();                            /* extern */
extern void *D_800C1BB0;

void func_00284254(void) {
    if (M2C_FIELD(D_800C1BB0, s32 *, 0x38) != 0) {
        func_00410CE8();
    }
}
