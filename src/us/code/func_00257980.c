#include "common.h"
#include "m2c_macros.h"

s32 func_00284188();                                /* extern */
s32 func_00284408();                                /* extern */
s32 func_00416AF8(M2C_UNK *);                       /* extern */
extern M2C_UNK D_80119E2C;

void func_00257980(void *arg0, void *arg1) {
    void *temp_a1;

    if ((func_00416AF8(&D_80119E2C) == 0) && (func_00284188() != 0) && (func_00284408() == 0x190) && (M2C_FIELD(arg0, s32 *, 0x34) == 0)) {
        temp_a1 = M2C_FIELD(arg1, void **, 0x51C);
        if ((temp_a1 != NULL) && (M2C_FIELD(temp_a1, s32 *, 0x54C) == 0)) {
            M2C_FIELD(arg0, s32 *, 0x34) = 1;
            M2C_FIELD(arg0, s32 *, 0x38) = 0;
        }
    }
}
