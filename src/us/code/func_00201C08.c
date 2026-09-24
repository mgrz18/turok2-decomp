#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0029B9B0(void *, s32);                 /* extern */
extern s32 D_800B6E7C;
extern s32 D_800B6E88;

void func_00201C08(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x238) != 0) {
        do {
            func_0029B9B0(arg0, D_800B6E88);
        } while (M2C_FIELD(arg0, s32 *, 0x238) != 0);
    }
    func_0029B9B0(arg0, D_800B6E7C);
}
