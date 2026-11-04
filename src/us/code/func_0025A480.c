#include "common.h"

s32 func_0026D5E4(s32, s32, s32);
s32 func_0026D628(s32);

s16 func_0025A600(s32, s32);                    /* extern */
s16 func_0025A684(s32, s32);                    /* extern */
s16 func_0025A810(s32, s32);                    /* extern */
s32 func_0026D5DC(s32 *);                   /* extern */

s16 func_0025A480(s32 arg0, s32 arg1) {
    u8 sp10[0x68];

    func_0026D5DC(sp10);
    func_0026D5E4((s32) sp10, (s32) func_0025A600(arg0, arg1), 0xA);
    func_0026D5E4((s32) sp10, (s32) func_0025A684(arg0, arg1), 0xA);
    func_0026D5E4((s32) sp10, (s32) func_0025A810(arg0, arg1), 0xA);
    return func_0026D628((s32) sp10);
}
