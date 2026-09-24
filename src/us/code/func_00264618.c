#include "common.h"
#include "m2c_macros.h"

void func_00264618(void *arg0, s32 *arg1) {
    if (!(M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0x38) & 0x80)) {
        *arg1 &= 0xEFFFFFFF;
    }
}
