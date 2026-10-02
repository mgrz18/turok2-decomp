#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_00431624;
extern M2C_UNK D_00431660;
extern M2C_UNK D_0043169C;
extern M2C_UNK D_00431700;
extern M2C_UNK D_8012FA24;

void func_00407158(s32 arg0) {
    M2C_FIELD(&D_8012FA24, s32 *, 0) = -1;
    M2C_FIELD(&D_8012FA24, s32 *, 4) = -1;
    if (arg0 == 0) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = NULL;
    }
    if (arg0 == 1) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = &D_00431660;
    }
    if (arg0 == 4) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = &D_00431700;
    }
    if (arg0 == 2) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = &D_00431624;
    }
    if (arg0 == 3) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = &D_0043169C;
    }
}
