#include "common.h"
#include "m2c_macros.h"

s32 func_00200518(s32, s32);
s32 func_00200738(s32, s32);
s32 func_0029DFF0(void);
void func_0029E010(s32);

void func_002020C8(s32 arg0, s32 arg1) {
    s32 temp_s2;

    temp_s2 = func_0029DFF0();
    func_00200738(arg0 + 0x507C, arg1);
    M2C_FIELD(arg1, s32 *, 0x14) = 0;
    func_00200518(arg0 + 0x5068, arg1);
    func_0029E010(temp_s2);
}
