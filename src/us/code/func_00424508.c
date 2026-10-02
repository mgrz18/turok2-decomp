#include "common.h"
#include "m2c_macros.h"

s32 func_00423E10(void);

extern s32 D_800C2038;

void func_00424508(s32 arg0) {
    D_800C2038 = arg0;
    func_00423E10();
}
