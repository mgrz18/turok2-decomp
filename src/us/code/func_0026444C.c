#include "common.h"
#include "m2c_macros.h"

void func_0026444C(void *arg0, s32 *arg1) {
    if (!(M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0x38) & 0x40)) {
        *arg1 &= 0xF7FFFFFF;
    }
}
