#include "common.h"
#include "m2c_macros.h"

void *func_0027FC2C();                              /* extern */

void *func_002815E0(void) {
    void *temp_v0;

    temp_v0 = func_0027FC2C();
    if (temp_v0 != NULL) {
        M2C_FIELD(temp_v0, s32 *, 0x20) = 1;
    }
    return temp_v0;
}
