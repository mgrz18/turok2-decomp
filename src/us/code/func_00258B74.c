#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);
s32 func_0024E700(s32, s32);
s32 func_00257BA0(s32);
s32 func_00257EA0(s32, s32);

extern u8 D_800ADF68[];

void func_00258B74(void *arg0, s32 arg1) {
    s32 temp_s2;
    void *temp_s0;

    temp_s0 = M2C_FIELD(arg0, void **, 0x1A8);
    temp_s2 = *(s16 *)(D_800ADF68 + (M2C_FIELD(temp_s0, s16 *, 0xA08) * 0x18));
    if (func_0024E700((s32) temp_s0, (s32) M2C_FIELD(temp_s0, s16 *, 0x996)) == 0) {
        M2C_FIELD(temp_s0, s16 *, 0xB14) = func_00257BA0((s32) temp_s0);
    }
    if ((func_00257EA0((s32) arg0, arg1) == 0) && !(M2C_FIELD(arg0, s32 *, 0xD4) & 0x400)) {
        func_00243414((s32) arg0, arg1, (s32) temp_s2);
    }
}
