#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_004313B4;
extern M2C_UNK D_00431424;
extern M2C_UNK D_004314CC;
extern M2C_UNK D_00431558;
extern M2C_UNK D_00431590;
extern M2C_UNK D_8012FA10;

void func_004070B4(s32 arg0) {
    M2C_FIELD(&D_8012FA10, s32 *, 0) = -1;
    M2C_FIELD(&D_8012FA10, s8 *, 8) = 1;
    M2C_FIELD(&D_8012FA10, s32 *, 4) = -1;
    if (arg0 == 0) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = NULL;
    }
    if (arg0 == 1) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_004313B4;
    }
    if (arg0 == 2) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_00431424;
    }
    if (arg0 == 3) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_004314CC;
    }
    if (arg0 == 4) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_00431558;
    }
    if (arg0 == 5) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_00431590;
    }
}
