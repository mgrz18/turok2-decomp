#include "common.h"
#include "m2c_macros.h"

void func_002928F0(void *arg0) {
    s32 temp_v1;
    s32 var_a1;

    var_a1 = 0;
    if (M2C_FIELD(arg0, u8 *, 0x34) != 0) {
        do {
            temp_v1 = var_a1 * 0x10;
            *(s32 *)(temp_v1 + M2C_FIELD(arg0, s32 *, 0x60)) = 0;
            M2C_FIELD((temp_v1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 6) = 0;
            M2C_FIELD((temp_v1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 0xA) = 0;
            M2C_FIELD((temp_v1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 7) = 0x40;
            M2C_FIELD((temp_v1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 9) = 0x7F;
            M2C_FIELD((temp_v1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 8) = 5;
            M2C_FIELD((temp_v1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 0xB) = 0;
            M2C_FIELD((temp_v1 + M2C_FIELD(arg0, s32 *, 0x60)), s16 *, 4) = 0xC8;
            M2C_FIELD((temp_v1 + M2C_FIELD(arg0, s32 *, 0x60)), f32 *, 0xC) = 1.0f;
            var_a1 += 1;
        } while (var_a1 < (s32) M2C_FIELD(arg0, u8 *, 0x34));
    }
}
