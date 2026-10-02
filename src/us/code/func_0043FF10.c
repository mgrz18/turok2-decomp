#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_00430F28;
extern M2C_UNK D_00430FB4;
extern M2C_UNK D_00431004;
extern M2C_UNK D_004310B8;
extern M2C_UNK D_00431158;
extern M2C_UNK D_00431220;
extern M2C_UNK D_8012F608;

void func_0043FF10(s32 arg0) {
    if (arg0 == 2) {
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == 1) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00430FB4;
        }
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == arg0) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_004310B8;
        }
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == 3) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00431220;
        }
        M2C_FIELD(&D_8012F608, s32 *, 0x3F8) = -1;
        M2C_FIELD(&D_8012F608, s32 *, 0x3FC) = -1;
    }
    if (arg0 == 1) {
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == arg0) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00430F28;
        }
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == 2) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00431004;
        }
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == 3) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00431158;
        }
        M2C_FIELD(&D_8012F608, s32 *, 0x3F8) = -1;
        M2C_FIELD(&D_8012F608, s32 *, 0x3FC) = -1;
    }
}
