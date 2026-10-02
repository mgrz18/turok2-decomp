#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_004312D4;
extern M2C_UNK D_00431344;
extern M2C_UNK D_004313EC;
extern M2C_UNK D_00431478;
extern M2C_UNK D_004314B0;
extern M2C_UNK D_8012FA10;

void func_004400B4(s32 arg0) {
    M2C_FIELD(&D_8012FA10, s32 *, 0) = -1;
    M2C_FIELD(&D_8012FA10, s8 *, 8) = 1;
    M2C_FIELD(&D_8012FA10, s32 *, 4) = -1;
    if (arg0 == 0) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = NULL;
    }
    if (arg0 == 1) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_004312D4;
    }
    if (arg0 == 2) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_00431344;
    }
    if (arg0 == 3) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_004313EC;
    }
    if (arg0 == 4) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_00431478;
    }
    if (arg0 == 5) {
        M2C_FIELD(&D_8012FA10, M2C_UNK **, -4) = &D_004314B0;
    }
}
