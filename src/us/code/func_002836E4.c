#include "common.h"
#include "m2c_macros.h"

M2C_UNK CList__AddTail(void *, s32);                 /* extern */
M2C_UNK CList__Remove(void *, s32);                 /* extern */

s32 func_002836E4(void *arg0) {
    s32 temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0x8A0);
    if (temp_s0 != 0) {
        CList__Remove(arg0 + 0x8A0, temp_s0);
        CList__AddTail(arg0 + 0x8B4, temp_s0);
    }
    return temp_s0;
}
