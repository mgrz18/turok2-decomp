#include "common.h"
#include "m2c_macros.h"

s32 func_00246860(s32, s32, s32);
s32 func_00275544(s32, s32, s32, s32, s32, s32);

extern f32 D_800C0508;
extern M2C_UNK D_8012F9D2;

void func_004072C8(void *arg0, s32 arg1) {
    M2C_FIELD(arg0, f32 *, 0x1FC) = (f32) D_800C0508;
    M2C_FIELD(&D_8012F9D2, s8 *, 0) = 0;
    M2C_FIELD(&D_8012F9D2, s32 *, 0x1A) = 0;
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2000);
    func_00275544(0x2779, M2C_FIELD(arg0, s32 *, 4), M2C_FIELD(arg0, s32 *, 8), M2C_FIELD(arg0, s32 *, 0xC), 0, -1);
    func_00246860((s32) arg0, arg1, 0x3B21);
}
