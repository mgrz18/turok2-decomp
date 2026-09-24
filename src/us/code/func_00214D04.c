#include "common.h"
#include "m2c_macros.h"

s32 func_00203CA0(M2C_UNK, s32, s32, s32, s32, void *, M2C_UNK *, M2C_UNK *, s32); /* extern */
M2C_UNK func_002051F4(M2C_UNK, s32);                /* extern */
M2C_UNK func_0020BE08(s32);                         /* extern */
s32 func_00214DBC();                                /* extern */
extern M2C_UNK D_800A57F8;
extern s32 D_800ACAD8;
extern M2C_UNK func_00214F14;

void func_00214D04(void *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    if (!(M2C_FIELD(arg0, u16 *, 0x88) & 0x40)) {
        temp_v0 = func_00214DBC();
        if (temp_v0 != 0) {
            func_0020BE08(temp_v0);
            temp_v0_2 = func_00203CA0(0, M2C_FIELD(arg0, s32 *, 0x80) | D_800ACAD8, temp_v0, M2C_FIELD(arg0, s32 *, 0x24), 8, arg0, &func_00214F14, &D_800A57F8, 0);
            if (temp_v0_2 != 0) {
                func_002051F4(0, temp_v0_2);
            }
            func_002051F4(0, temp_v0);
        }
    }
}
