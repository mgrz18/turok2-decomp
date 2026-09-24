#include "common.h"
#include "m2c_macros.h"

void *func_0022425C(M2C_UNK *, s32);                /* extern */
extern M2C_UNK D_800F7078;

u8 func_0026C0E0(s32 arg0) {
    void *temp_v0;

    temp_v0 = func_0022425C(&D_800F7078, arg0);
    if (temp_v0 == NULL) {
        return 0U;
    }
    return M2C_FIELD(temp_v0, u8 *, 0x58);
}
