#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800A8078;
extern f32 D_800A8080;

f32 func_0026EF30(void *arg0) {
    s32 temp_a1;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0);
    temp_a1 = M2C_FIELD(temp_v1, s32 *, 0);
    switch (temp_a1) {                              /* irregular */
    case 0:
        return (f32) M2C_FIELD((temp_v1 + (s32) ((M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(&D_800A8078, f32 *, 4)) / M2C_FIELD(temp_v1, f32 *, 0x14))), u8 *, 4) * D_800A8080;
    case 1:
        return M2C_FIELD(arg0, f32 *, 8);
    default:
        return 0.0f;
    }
}
