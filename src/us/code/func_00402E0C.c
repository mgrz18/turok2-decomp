#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002666B0(s32, s32, M2C_UNK);           /* extern */
extern s32 D_8012F5C0;

void func_00402E0C(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & ~0x2000);
    func_002666B0(D_8012F5C0, D_8012F5C0 + 0x140, 0x5ABE);
}
