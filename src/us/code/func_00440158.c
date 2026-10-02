#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_00431544;
extern M2C_UNK D_00431580;
extern M2C_UNK D_004315BC;
extern M2C_UNK D_00431620;
extern M2C_UNK D_8012FA24;

void func_00440158(s32 arg0) {
    M2C_FIELD(&D_8012FA24, s32 *, 0) = -1;
    M2C_FIELD(&D_8012FA24, s32 *, 4) = -1;
    if (arg0 == 0) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = NULL;
    }
    if (arg0 == 1) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = &D_00431580;
    }
    if (arg0 == 4) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = &D_00431620;
    }
    if (arg0 == 2) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = &D_00431544;
    }
    if (arg0 == 3) {
        M2C_FIELD(&D_8012FA24, M2C_UNK **, -4) = &D_004315BC;
    }
}
