#include "common.h"
#include "m2c_macros.h"

M2C_UNK CList__AddHead(s32, void *);                 /* extern */
M2C_UNK CList__Remove();                            /* extern */

void func_00228B4C(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s16 *, 0x16) = 0;
    CList__Remove();
    CList__AddHead(arg0 + 0x14, arg1);
}
