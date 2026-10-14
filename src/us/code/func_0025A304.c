#include "common.h"
#include "m2c_macros.h"

s32 func_0026D5E4(s32, s32, s32);
s32 func_0026D628(s32);

s16 func_0025A5A8(void *, M2C_UNK);                 /* extern */
s16 func_0025A62C(void *, M2C_UNK);                 /* extern */
s16 func_0025A7B8(void *, M2C_UNK);                 /* extern */
M2C_UNK func_0026D5DC(M2C_UNK *);                   /* extern */

s16 func_0025A304(void *arg0, M2C_UNK arg1) {
    u8 sp10[0x68];
    void *temp_s0;

    temp_s0 = M2C_FIELD(arg0, s32 *, 0x14) + 0x14;
    func_0026D5DC(sp10);
    if (M2C_FIELD(temp_s0, u16 *, 0x6A) & 0x1F) {
        func_0026D5E4((s32) sp10, (s32) func_0025A5A8(arg0, arg1), 0xA);
    }
    if (M2C_FIELD(temp_s0, u16 *, 0x6C) & 0x1F) {
        func_0026D5E4((s32) sp10, (s32) func_0025A62C(arg0, arg1), 0xA);
    }
    if (M2C_FIELD(temp_s0, u16 *, 0x72) & 0x1F) {
        func_0026D5E4((s32) sp10, (s32) func_0025A7B8(arg0, arg1), 0xA);
    }
    return func_0026D628((s32) sp10);
}
