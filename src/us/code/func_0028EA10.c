#include "common.h"
#include "m2c_macros.h"

void func_0028EA10(void *arg0, void *arg1) {
    void *temp_v0;

    M2C_FIELD(arg1, s32 *, 4) = 0;
    M2C_FIELD(arg1, void **, 0) = (void *) M2C_FIELD(arg0, void **, 0);
    temp_v0 = M2C_FIELD(arg0, void **, 0);
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, void **, 4) = arg1;
    }
    M2C_FIELD(arg0, void **, 0) = arg1;
    if (M2C_FIELD(arg0, void **, 4) == NULL) {
        M2C_FIELD(arg0, void **, 4) = arg1;
    }
}
