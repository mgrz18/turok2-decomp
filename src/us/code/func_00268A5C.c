#include "common.h"
#include "m2c_macros.h"

void *func_0022425C(M2C_UNK *, void *);             /* extern */
extern M2C_UNK D_800F7078;

f32 func_00268A5C(void *arg0) {
    void *temp_a1;
    void *temp_v0;

    temp_a1 = M2C_FIELD(arg0, void **, 0x10);
    if ((temp_a1 != NULL) && (M2C_FIELD(temp_a1, u16 *, 2) & 1)) {
        temp_v0 = func_0022425C(&D_800F7078, temp_a1);
        if (temp_v0 != NULL) {
            return M2C_FIELD(temp_v0, f32 *, 0x20);
        }
    }
    return 0.0f;
}
