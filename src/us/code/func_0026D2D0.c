#include "common.h"
#include "m2c_macros.h"

void func_0026D2D0(void *arg0, void *arg1) {
    void **temp_v1;
    void *temp_v1_2;

    temp_v1 = M2C_FIELD(arg1, void ***, 4);
    if (temp_v1 != NULL) {
        *temp_v1 = M2C_FIELD(arg1, void **, 0);
    }
    temp_v1_2 = M2C_FIELD(arg1, void **, 0);
    if (temp_v1_2 != NULL) {
        M2C_FIELD(temp_v1_2, void ***, 4) = (void **) M2C_FIELD(arg1, void ***, 4);
    }
    if (M2C_FIELD(arg0, void ***, 0) == arg1) {
        M2C_FIELD(arg0, void ***, 0) = (void **) M2C_FIELD(arg1, void ***, 4);
    }
    if (M2C_FIELD(arg0, void **, 4) == arg1) {
        M2C_FIELD(arg0, void **, 4) = (void *) M2C_FIELD(arg1, void **, 0);
    }
    M2C_FIELD(arg0, s32 *, 8) = (s32) (M2C_FIELD(arg0, s32 *, 8) - 1);
}
