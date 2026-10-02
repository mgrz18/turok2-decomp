#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0042A9A0();                            /* extern */
extern s32 D_800AB8A0;

void func_00465678(void *arg0) {
    func_0042A9A0();
    D_800AB8A0 = ((u8) M2C_FIELD(M2C_FIELD(arg0, void **, 0x18E50), u8 *, 0x10) >> 2) & 1;
}
