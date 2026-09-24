#include "common.h"
#include "m2c_macros.h"

void *func_0022425C(M2C_UNK *, u16 *);              /* extern */
M2C_UNK func_0026C9B8(u16 *, u16);                  /* extern */
extern M2C_UNK D_800F7078;

void func_0026C7A8(u16 *arg0) {
    void *temp_v0;

    temp_v0 = func_0022425C(&D_800F7078, arg0);
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, s32 *, 0x44) = (s32) (M2C_FIELD(temp_v0, s32 *, 0x44) | 0x4000);
    }
    if (arg0 != NULL) {
        func_0026C9B8(arg0, *arg0);
    }
}
