#include "common.h"
#include "m2c_macros.h"

extern u8 D_8011AAF1;

s32 func_00267EA8(void *arg0) {
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg0, void **, 0x14);
    if (M2C_FIELD(temp_a0, s32 *, 0) == 0xB) {
        if (D_8011AAF1 != 0) {
            return M2C_FIELD(temp_a0, s32 *, 0x14) & 1;
        }
        return 1;
    }
    return 0;
}
