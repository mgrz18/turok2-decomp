#include "common.h"
#include "m2c_macros.h"

s32 func_0026D5E4(s32, s32, s32);
s32 func_0026D628(s32);

s16 func_0025A600(s32, M2C_UNK);                    /* extern */
s16 func_0025A684(s32, M2C_UNK);                    /* extern */
s16 func_0025A810(s32, M2C_UNK);                    /* extern */
M2C_UNK func_0026D5DC(M2C_UNK *);                   /* extern */

s16 func_0025A480(s32 arg0, M2C_UNK arg1) {
    u8 sp10[0x68];

    func_0026D5DC(sp10);
    func_0026D5E4((s32) sp10, (s32) func_0025A600(arg0, arg1), 0xA);
    func_0026D5E4((s32) sp10, (s32) func_0025A684(arg0, arg1), 0xA);
    func_0026D5E4((s32) sp10, (s32) func_0025A810(arg0, arg1), 0xA);
    return func_0026D628((s32) sp10);
}
