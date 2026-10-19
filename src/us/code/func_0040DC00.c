#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

f32 func_002119FC(f32 arg0, f32 arg1);

extern f32 D_800C0900;
extern f32 D_800C0904;

void func_0040DC00(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, f32 *, 0x64) = func_002119FC(D_800C0900, D_800C0904);
}
