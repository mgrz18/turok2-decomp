#include "common.h"
#include "m2c_macros.h"

void func_00281C08(void *arg0) {
    void *temp_a0;
    void *temp_v0;
    void *temp_v0_2;

    temp_v0 = arg0 + 0x18;
    M2C_FIELD(arg0, s32 *, 8) = 0;
    M2C_FIELD(arg0, s32 *, 0xC) = 0;
    M2C_FIELD(arg0, s32 *, 0x10) = 0;
    M2C_FIELD(arg0, f32 *, 0x14) = 1.0f;
    M2C_FIELD(temp_v0, s32 *, 4) = 0;
    M2C_FIELD(temp_v0, s32 *, 8) = 0;
    M2C_FIELD(temp_v0, s32 *, 0xC) = 0;
    M2C_FIELD(temp_v0, s32 *, 0x10) = 0;
    temp_v0_2 = arg0 + 0x2C;
    temp_a0 = arg0 + 0x40;
    M2C_FIELD(temp_v0_2, s32 *, 4) = 0;
    M2C_FIELD(temp_v0_2, s32 *, 8) = 0;
    M2C_FIELD(temp_v0_2, s32 *, 0xC) = 0;
    M2C_FIELD(temp_v0_2, s32 *, 0x10) = 0;
    M2C_FIELD(temp_a0, s32 *, 4) = 0;
    M2C_FIELD(temp_a0, s32 *, 8) = 0;
    M2C_FIELD(temp_a0, s32 *, 0xC) = 0;
    M2C_FIELD(temp_a0, s32 *, 0x10) = 0;
}
