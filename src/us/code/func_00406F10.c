#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_00431008;
extern M2C_UNK D_00431094;
extern M2C_UNK D_004310E4;
extern M2C_UNK D_00431198;
extern M2C_UNK D_00431238;
extern M2C_UNK D_00431300;
extern M2C_UNK D_8012F608;

void func_00406F10(s32 arg0) {
    if (arg0 == 2) {
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == 1) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00431094;
        }
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == arg0) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00431198;
        }
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == 3) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00431300;
        }
        M2C_FIELD(&D_8012F608, s32 *, 0x3F8) = -1;
        M2C_FIELD(&D_8012F608, s32 *, 0x3FC) = -1;
    }
    if (arg0 == 1) {
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == arg0) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00431008;
        }
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == 2) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_004310E4;
        }
        if (M2C_FIELD(&D_8012F608, s32 *, 0) == 3) {
            M2C_FIELD(&D_8012F608, M2C_UNK **, 0x3F4) = &D_00431238;
        }
        M2C_FIELD(&D_8012F608, s32 *, 0x3F8) = -1;
        M2C_FIELD(&D_8012F608, s32 *, 0x3FC) = -1;
    }
}
