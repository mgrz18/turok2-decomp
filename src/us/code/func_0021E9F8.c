#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0021E960();                            /* extern */

s32 func_0021E9F8(void *arg0, s32 arg1, s32 arg2) {
    if ((arg1 < 0) || ((arg2 == 0) && (M2C_FIELD(arg0, s32 *, 0xD4) & 0x400))) {
        return 0;
    }
    func_0021E960();
    return 1;
}
