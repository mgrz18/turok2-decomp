#include "common.h"
#include "m2c_macros.h"

void func_004172B0(void *arg0) {
    M2C_UNK (*temp_v0)();
    void *temp_v0_2;

    temp_v0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), M2C_UNK (**)(), 0xC);
    if (temp_v0 != NULL) {
        temp_v0();
    }
    temp_v0_2 = M2C_FIELD(arg0, void **, 0x20);
    M2C_FIELD(temp_v0_2, s32 *, 0xB0) = 0;
    M2C_FIELD(temp_v0_2, s32 *, 0xB4) = 0;
    M2C_FIELD(temp_v0_2, s32 *, 0xBC) = 0;
}
