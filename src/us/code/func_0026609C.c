#include "common.h"
#include "m2c_macros.h"

void func_0026609C(s32 arg0, void *arg1) {
    M2C_UNK (*temp_v0_2)();
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg1, void **, 0x30);
    if (temp_v0 != NULL) {
        temp_v0_2 = M2C_FIELD(temp_v0, M2C_UNK (**)(), 8);
        if (temp_v0_2 != NULL) {
            temp_v0_2();
        }
    }
}
