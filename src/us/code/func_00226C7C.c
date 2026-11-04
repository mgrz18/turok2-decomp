#include "common.h"

/* Only the field read here: a resource walked with func_002017D4. */
typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 unk6C;
} TableOwner;

s32 func_002017D4(s32, s32);                        /* extern */

s32 func_00226C7C(TableOwner *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = func_002017D4(arg0->unk6C, 1);
    func_002017D4(temp_v0, 0);
    return func_002017D4(temp_v0, 1) + arg1;
}
