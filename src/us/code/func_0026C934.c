#include "common.h"
#include "m2c_macros.h"

void func_0026C934(void *arg0, u16 arg1) {
    u16 temp_s0;
    u16 temp_v1;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;

    temp_s0 = M2C_FIELD(arg0, u16 *, 0);
    if (temp_s0 == (arg1 & 0xFFFF)) {
        temp_v1 = M2C_FIELD(arg0, u16 *, 2);
        if (temp_v1 & 4) {
            temp_a0 = M2C_FIELD(arg0, void **, 0x10);
            M2C_FIELD(arg0, u16 *, 2) = (u16) (temp_v1 & 0xFFFB);
            if (temp_a0 != NULL) {
                func_0026C934(temp_a0, temp_s0);
            }
            temp_a0_2 = M2C_FIELD(arg0, void **, 0x14);
            if (temp_a0_2 != NULL) {
                func_0026C934(temp_a0_2, temp_s0);
            }
            temp_a0_3 = M2C_FIELD(arg0, void **, 0x18);
            if (temp_a0_3 != NULL) {
                func_0026C934(temp_a0_3, temp_s0);
            }
        }
    }
}
