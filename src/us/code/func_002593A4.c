#include "common.h"
#include "m2c_macros.h"

f32 func_00210894(s32, s32);

void func_002593A4(s32 arg0, void *arg1) {
    if (M2C_FIELD(arg1, s32 *, 0x90) == 2) {
        func_00210894(arg0, M2C_FIELD(M2C_FIELD(arg1, void **, 0), s32 *, 0x24C));
    }
}
