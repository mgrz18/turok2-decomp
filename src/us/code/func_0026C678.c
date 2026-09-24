#include "common.h"
#include "m2c_macros.h"

void *func_0022425C(M2C_UNK *, void *);             /* extern */
M2C_UNK func_0026CEC8(s32);                         /* extern */
extern M2C_UNK D_800F7078;

void func_0026C678(void *arg0) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    u16 temp_v1;
    void *temp_v0;

    if (arg0 != NULL) {
        temp_v1 = M2C_FIELD(arg0, u16 *, 2);
        if (temp_v1 & 1) {
            temp_a0 = M2C_FIELD(arg0, s32 *, 0x10);
            M2C_FIELD(arg0, u16 *, 2) = (u16) (temp_v1 & 0xFFFE);
            if (temp_a0 != 0) {
                func_0026CEC8(temp_a0);
            }
            temp_a0_2 = M2C_FIELD(arg0, s32 *, 0x14);
            if (temp_a0_2 != 0) {
                func_0026CEC8(temp_a0_2);
            }
            temp_a0_3 = M2C_FIELD(arg0, s32 *, 0x18);
            if (temp_a0_3 != 0) {
                func_0026CEC8(temp_a0_3);
            }
        }
        temp_v0 = func_0022425C(&D_800F7078, arg0);
        if (temp_v0 != NULL) {
            M2C_FIELD(temp_v0, s8 *, 0x58) = 0;
            M2C_FIELD(temp_v0, s8 *, 0x5C) = 0;
            M2C_FIELD(temp_v0, s32 *, 0x44) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x44) & ~1);
            M2C_FIELD(temp_v0, u8 *, 0x5B) = (u8) (M2C_FIELD(temp_v0, u8 *, 0x5B) & 0xFD);
        }
    }
}
