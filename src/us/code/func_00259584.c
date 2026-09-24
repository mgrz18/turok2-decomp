#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00243414(void *, void *, M2C_UNK);     /* extern */
s32 func_0024E700(void *, s16);                     /* extern */

void func_00259584(void *arg0, void *arg1) {
    void *temp_s0;

    temp_s0 = M2C_FIELD(arg0, void **, 0x1A8);
    if (func_0024E700(temp_s0, M2C_FIELD(temp_s0, s16 *, 0x996)) == 0) {
        func_00243414(arg0, arg1, 2);
        return;
    }
    if (M2C_FIELD(temp_s0, s32 *, 0xA68) & 0x10) {
        func_00243414(arg0, arg1, 5);
        return;
    }
    if (!(M2C_FIELD(temp_s0, s32 *, 0xA64) & 0x2000)) {
        func_00243414(arg0, arg1, 2);
    }
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        M2C_FIELD(arg1, s8 *, 0x35) = -1;
    }
}
