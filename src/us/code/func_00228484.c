#include "common.h"
#include "m2c_macros.h"

M2C_UNK CList__AddTail(s32, void *);                 /* extern */
M2C_UNK CList__Remove(s32, void *);                 /* extern */

void func_00228484(s32 arg0, void *arg1) {
    s32 *temp_v1;

    temp_v1 = M2C_FIELD(arg1, s32 **, 0x290);
    M2C_FIELD(arg1, s32 *, 0x144) = 0;
    if (temp_v1 != NULL) {
        *temp_v1 -= 1;
    }
    CList__Remove(arg0 + 0x5314, arg1);
    CList__AddTail(arg0 + 0x5300, arg1);
}
