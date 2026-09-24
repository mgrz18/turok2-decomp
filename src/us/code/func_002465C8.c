#include "common.h"
#include "m2c_macros.h"

void func_002465C8(s32 arg0, void *arg1) {
    M2C_UNK (*temp_v0)();

    temp_v0 = M2C_FIELD(arg1, M2C_UNK (**)(), 0xF8);
    if (temp_v0 != NULL) {
        temp_v0();
    }
}
