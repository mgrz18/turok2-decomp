#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00220260(M2C_UNK *, void *, M2C_UNK);  /* extern */
extern M2C_UNK D_800F7078;

void func_002648BC(void *arg0) {
    func_00220260(&D_800F7078, arg0, 0);
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & ~0x100);
}
