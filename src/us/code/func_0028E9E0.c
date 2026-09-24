#include "common.h"
#include "m2c_macros.h"

void func_0028E9E0(void *arg0, M2C_UNK *arg1) {
    M2C_UNK *temp_v0;

    M2C_FIELD(arg1, M2C_UNK **, 0) = NULL;
    M2C_FIELD(arg1, M2C_UNK **, 4) = (M2C_UNK *) M2C_FIELD(arg0, M2C_UNK **, 4);
    temp_v0 = M2C_FIELD(arg0, M2C_UNK **, 4);
    if (temp_v0 != NULL) {
        *temp_v0 = arg1;
    }
    M2C_FIELD(arg0, M2C_UNK **, 4) = arg1;
    if (M2C_FIELD(arg0, M2C_UNK **, 0) == NULL) {
        M2C_FIELD(arg0, M2C_UNK **, 0) = arg1;
    }
}
