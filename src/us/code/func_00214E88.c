#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00205A2C(M2C_UNK, s32);                /* extern */
extern s32 D_800ACAD8;
extern u8 D_800B6D1B;

void func_00214E88(void *arg0) {
    if (!(M2C_FIELD(arg0, u16 *, 0x88) & 0x40) && (M2C_FIELD(arg0, u8 *, 0x8A) != D_800B6D1B)) {
        func_00205A2C(0, M2C_FIELD(arg0, s32 *, 0x80) | D_800ACAD8);
    }
}
