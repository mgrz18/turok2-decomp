#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00228B4C(M2C_UNK *, s32);              /* extern */
M2C_UNK func_002327F4();                            /* extern */
M2C_UNK func_00234E48(M2C_UNK *, void *);           /* extern */
extern M2C_UNK D_8010FF34;
extern M2C_UNK D_801102B8;

void func_00231E94(void *arg0) {
    s32 temp_a1;

    if (M2C_FIELD(arg0, u8 *, 0x1B5) != 0) {
        func_002327F4();
        func_00234E48(&D_801102B8, arg0);
    }
    temp_a1 = M2C_FIELD(arg0, s32 *, 0x118);
    if (temp_a1 != 0) {
        func_00228B4C(&D_8010FF34, temp_a1);
        M2C_FIELD(arg0, s32 *, 0x118) = 0;
    }
}
