#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0021A7EC(void *);                      /* extern */
M2C_UNK func_00220260(M2C_UNK *, void *, M2C_UNK);  /* extern */
s32 func_00284188();                                /* extern */
extern M2C_UNK D_800F7078;

void func_00266994(void *arg0, s32 *arg1) {
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2100);
    if (func_00284188() != 0) {
        *arg1 |= 0x200;
    }
    func_0021A7EC(arg0);
    func_00220260(&D_800F7078, arg0, 0);
}
