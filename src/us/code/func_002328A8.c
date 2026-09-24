#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0020B9F4(s32, void *, void *, M2C_UNK, s32); /* extern */
M2C_UNK func_002254C0(M2C_UNK *, s32, s32);         /* extern */
extern s32 D_800B6D1C;
extern M2C_UNK D_800F7078;

void func_002328A8(void *arg0) {
    func_002254C0(&D_800F7078, arg0 + 4, arg0 + ((D_800B6D1C * 0x18) + 0xC8));
    func_0020B9F4(M2C_FIELD(M2C_FIELD(arg0, void **, 0xF8), s32 *, 0x14), arg0 + ((D_800B6D1C << 6) + 0x48), arg0 + ((D_800B6D1C * 0x18) + 0xC8), 0, -1);
}
