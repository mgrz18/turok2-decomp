#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0022F1C8(M2C_UNK *, void *);           /* extern */
M2C_UNK func_00232B40(void *);                      /* extern */
s16 *func_0025398C(s32, M2C_UNK);                   /* extern */
extern M2C_UNK D_800F8928;

void func_0023181C(void *arg0) {
    s32 temp_s0;
    u16 temp_v1;
    u8 temp_a0;
    void *temp_s0_2;

    if (M2C_FIELD(arg0, u16 *, 2) == 0x68) {
        temp_s0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x10C), s32 *, 0x1A8);
        M2C_FIELD(((*func_0025398C(temp_s0, 0x15) * 2) + temp_s0), s16 *, 0x934) = 1;
    }
    temp_s0_2 = M2C_FIELD(arg0, void **, 0x108);
    if (temp_s0_2 != NULL) {
        temp_a0 = M2C_FIELD(temp_s0_2, u8 *, 0);
        if ((temp_a0 == 2) && (M2C_FIELD(temp_s0_2, s32 *, 0x40) & 0x100) && ((temp_v1 = M2C_FIELD(temp_s0_2, u16 *, 2), (temp_v1 == temp_a0)) || (temp_v1 == 0x111))) {
            func_0022F1C8(&D_800F8928, temp_s0_2);
            func_00232B40(temp_s0_2);
        }
    }
    func_00232B40(arg0);
}
