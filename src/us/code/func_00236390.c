#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800AB8B0;
extern M2C_UNK D_800AB8C0;

void func_00236390(void *arg0) {
    void *temp_v0;

    if (M2C_FIELD(arg0, f32 *, 0x24) != 0.0f) {
        temp_v0 = (void *)((s8 *)&D_800AB8B0 + 4);
        M2C_FIELD(&D_800AB8B0, f32 *, 4) = (f32) (M2C_FIELD(&D_800AB8B0, f32 *, 4) + M2C_FIELD(arg0, f32 *, 0x34));
        M2C_FIELD(temp_v0, f32 *, 4) = (f32) (M2C_FIELD(temp_v0, f32 *, 4) + M2C_FIELD(arg0, f32 *, 0x38));
        M2C_FIELD(temp_v0, f32 *, 8) = (f32) (M2C_FIELD(temp_v0, f32 *, 8) + M2C_FIELD(arg0, f32 *, 0x3C));
        M2C_FIELD(temp_v0, f32 *, 0xC) = (f32) (M2C_FIELD(temp_v0, f32 *, 0xC) + M2C_FIELD(arg0, f32 *, 0x24));
    }
    M2C_FIELD(&D_800AB8C0, f32 *, 4) = (f32) (M2C_FIELD(&D_800AB8C0, f32 *, 4) + M2C_FIELD(arg0, f32 *, 0x14));
}
