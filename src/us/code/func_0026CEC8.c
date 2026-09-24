#include "common.h"
#include "m2c_macros.h"

void func_0026CEC8(void *arg0) {
    u16 temp_v1;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;

    temp_v1 = M2C_FIELD(arg0, u16 *, 2);
    if (temp_v1 & 1) {
        temp_a0 = M2C_FIELD(arg0, void **, 0x10);
        M2C_FIELD(arg0, u16 *, 2) = (u16) (temp_v1 & 0xFFFE);
        if (temp_a0 != NULL) {
            func_0026CEC8(temp_a0);
        }
        temp_a0_2 = M2C_FIELD(arg0, void **, 0x14);
        if (temp_a0_2 != NULL) {
            func_0026CEC8(temp_a0_2);
        }
        temp_a0_3 = M2C_FIELD(arg0, void **, 0x18);
        if (temp_a0_3 != NULL) {
            func_0026CEC8(temp_a0_3);
        }
    }
}
