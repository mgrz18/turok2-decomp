#include "common.h"
#include "m2c_macros.h"

s32 func_00293EC0(s32);

s32 func_00293580(void *);                          /* extern */
M2C_UNK func_00293E60(void *, s16);                 /* extern */

void func_00274850(void *arg0) {
    void *temp_a0;
    void *temp_s0;

    temp_a0 = M2C_FIELD(arg0, void **, 0xB0);
    M2C_FIELD(arg0, s32 *, 0xAC) = 1;
    M2C_FIELD(arg0, s32 *, 0x50) = 0;
    temp_s0 = temp_a0 + 0x84;
    if (M2C_FIELD(arg0, s32 *, 0x10) != M2C_FIELD(temp_a0, s32 *, 0x104)) {
        func_00293E60(temp_s0, *(s16 *)((s8 *)temp_a0 + (M2C_FIELD(arg0, s32 *, 0) << 1) + 0xDC));
        if (func_00293580(temp_s0) != 0) {
            func_00293EC0((s32) temp_s0);
        }
        M2C_FIELD(arg0, s32 *, 4) = -1;
    }
}
