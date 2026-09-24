#include "common.h"
#include "m2c_macros.h"

M2C_UNK *func_0029247C(void *arg0, s8 arg1, s8 arg2, s8 arg3) {
    M2C_UNK *temp_v1;

    temp_v1 = M2C_FIELD(arg0, M2C_UNK **, 0x6C);
    if (temp_v1 != NULL) {
        M2C_FIELD(arg0, M2C_UNK **, 0x6C) = (M2C_UNK *) M2C_FIELD(temp_v1, M2C_UNK **, 0);
        M2C_FIELD(temp_v1, M2C_UNK **, 0) = NULL;
        if (M2C_FIELD(arg0, M2C_UNK **, 0x64) == NULL) {
            M2C_FIELD(arg0, M2C_UNK **, 0x64) = temp_v1;
        } else {
            *M2C_FIELD(arg0, M2C_UNK **, 0x68) = temp_v1;
        }
        M2C_FIELD(arg0, M2C_UNK **, 0x68) = temp_v1;
        M2C_FIELD(temp_v1, s8 *, 0x31) = arg3;
        M2C_FIELD(temp_v1, s8 *, 0x32) = arg1;
        M2C_FIELD(temp_v1, s8 *, 0x33) = arg2;
        M2C_FIELD(temp_v1, M2C_UNK **, 0x14) = temp_v1;
    }
    return temp_v1;
}
