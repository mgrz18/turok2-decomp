#include "common.h"
#include "m2c_macros.h"

s32 func_00265F90(s32, s32, s32);                   /* extern */
extern s32 D_800F7158;
extern s32 D_80110020;

s32 func_0026E6D4(void *arg0, s32 arg1) {
    s32 temp_a1;

    if (M2C_FIELD(arg0, u16 *, 0xA) == D_80110020) {
        if (arg1 != 0) {
            temp_a1 = D_800F7158 + (M2C_FIELD(arg0, u16 *, 4) * 0x288);
            if ((M2C_FIELD(arg0, u8 *, 0x11) == 0xA) && (M2C_FIELD(arg0, u8 *, 0x12) == 1)) {
                return func_00265F90(temp_a1, temp_a1 + 0x140, arg1);
            }
        }
        /* Duplicate return node #6. Try simplifying control flow for better match */
        return 1;
    }
    return 1;
}
