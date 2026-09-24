#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200574(M2C_UNK *, void *);           /* extern */
M2C_UNK func_00200738(M2C_UNK *, void *);           /* extern */
extern M2C_UNK D_8010FEA0;
extern M2C_UNK D_8010FEB4;

void func_002280B0(void *arg0) {
    s32 *temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 **, 0x290);
    M2C_FIELD(arg0, s32 *, 0x144) = 0;
    if (temp_v1 != NULL) {
        *temp_v1 -= 1;
    }
    func_00200738(&D_8010FEB4, arg0);
    func_00200574(&D_8010FEA0, arg0);
}
