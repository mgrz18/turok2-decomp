#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_00431700;
extern M2C_UNK D_8012F9D0;

void func_00407BBC(void) {
    M2C_FIELD(&D_8012F9D0, s8 *, 0) = 1;
    M2C_FIELD(&D_8012F9D0, s32 *, 0x54) = -1;
    M2C_FIELD(&D_8012F9D0, s32 *, 0x58) = -1;
    M2C_FIELD(&D_8012F9D0, M2C_UNK **, 0x50) = &D_00431700;
}
