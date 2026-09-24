#include "common.h"
#include "m2c_macros.h"

void func_0026CC0C(void *arg0) {
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;

    temp_a0 = M2C_FIELD(arg0, void **, 0x10);
    M2C_FIELD(arg0, u16 *, 2) = (u16) (M2C_FIELD(arg0, u16 *, 2) & 0x7FFF);
    if ((temp_a0 != NULL) && (M2C_FIELD(temp_a0, u16 *, 2) & 0x8000)) {
        func_0026CC0C(temp_a0);
    }
    temp_a0_2 = M2C_FIELD(arg0, void **, 0x14);
    if ((temp_a0_2 != NULL) && (M2C_FIELD(temp_a0_2, u16 *, 2) & 0x8000)) {
        func_0026CC0C(temp_a0_2);
    }
    temp_a0_3 = M2C_FIELD(arg0, void **, 0x18);
    if ((temp_a0_3 != NULL) && (M2C_FIELD(temp_a0_3, u16 *, 2) & 0x8000)) {
        func_0026CC0C(temp_a0_3);
    }
}
