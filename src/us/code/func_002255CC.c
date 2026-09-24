#include "common.h"
#include "m2c_macros.h"

s32 func_00267090(s32);
s32 func_002681BC(s32);

M2C_UNK func_0021EBC0(s32);                         /* extern */

void func_002255CC(void *arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;

    func_0021EBC0(arg1);
    if (func_00267090(arg1) != 0) {
        temp_v1 = M2C_FIELD(arg0, s32 *, 0x8EC);
        if (temp_v1 != 0x200) {
            M2C_FIELD((arg0 + ((temp_v1 << 2))), s32 *, 0xEC) = arg1;
            M2C_FIELD(arg0, s32 *, 0x8EC) = (s32) (temp_v1 + 1);
        }
    }
    if (func_002681BC(arg1) != 0) {
        temp_v1_2 = M2C_FIELD(arg0, s32 *, 0xAF0);
        if (temp_v1_2 != 0x80) {
            M2C_FIELD((arg0 + ((temp_v1_2 << 2))), s32 *, 0x8F0) = arg1;
            M2C_FIELD(arg0, s32 *, 0xAF0) = (s32) (temp_v1_2 + 1);
        }
    }
    temp_v1_3 = M2C_FIELD(arg0, s32 *, 0xDF8);
    if (temp_v1_3 < 0x80) {
        M2C_FIELD((arg0 + ((temp_v1_3 << 2))), s32 *, 0xBF8) = arg1;
        M2C_FIELD(arg0, s32 *, 0xDF8) = (s32) (temp_v1_3 + 1);
    }
    temp_a0 = M2C_FIELD(arg0, s32 *, 0xEFC);
    if ((temp_a0 < 0x40) && (*M2C_FIELD(arg1, s32 **, 0x14) == 1)) {
        M2C_FIELD((arg0 + ((temp_a0 << 2))), s32 *, 0xDFC) = arg1;
        temp_v0 = M2C_FIELD(arg0, s32 *, 0xEFC);
        M2C_FIELD(arg1, s8 *, 0x20A) = (s8) temp_v0;
        M2C_FIELD(arg0, s32 *, 0xEFC) = (s32) (temp_v0 + 1);
    }
    temp_v1_4 = M2C_FIELD(arg0, s32 *, 0xF84);
    if ((temp_v1_4 < 0x20) && (M2C_FIELD(arg1, s32 *, 0xD4) & 2)) {
        M2C_FIELD((arg0 + ((temp_v1_4 << 2))), s32 *, 0xF04) = arg1;
        M2C_FIELD(arg0, s32 *, 0xF84) = (s32) (temp_v1_4 + 1);
    }
    temp_a0_2 = M2C_FIELD(arg0, s32 *, 0xFC8);
    if ((temp_a0_2 < 0x10) && (*M2C_FIELD(arg1, s32 **, 0x14) == 5)) {
        M2C_FIELD((arg0 + ((temp_a0_2 << 2))), s32 *, 0xF88) = arg1;
        M2C_FIELD(arg0, s32 *, 0xFC8) = (s32) (temp_a0_2 + 1);
    }
    temp_a0_3 = M2C_FIELD(arg0, s32 *, 0x104C);
    if (temp_a0_3 < 0x20) {
        if (M2C_FIELD(arg1, u16 *, 0xB8) == 0x64F) {
            M2C_FIELD((arg0 + ((temp_a0_3 << 2))), s32 *, 0xFCC) = arg1;
            M2C_FIELD(arg0, s32 *, 0x104C) = (s32) (temp_a0_3 + 1);
        }
    }
    if (M2C_FIELD(arg1, u16 *, 0xB8) == 0x64D) {
        temp_v1_5 = M2C_FIELD(arg0, s32 *, 0x1060);
        if (temp_v1_5 < 4) {
            M2C_FIELD((arg0 + ((temp_v1_5 << 2))), s32 *, 0x1050) = arg1;
            M2C_FIELD(arg0, s32 *, 0x1060) = (s32) (temp_v1_5 + 1);
        }
    }
}
