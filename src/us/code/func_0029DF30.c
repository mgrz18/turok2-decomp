#include "common.h"
#include "m2c_macros.h"

s32 func_0029DFF0(void);
void func_0029E010(s32);

extern s32 D_800B8908[];

void func_0029DF30(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0029DFF0();
    D_800B8908[0] |= arg0;
    func_0029E010(temp_v0);
}
