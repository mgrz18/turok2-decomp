#include "common.h"

s32 func_0026D288(s32, s32);                    /* extern */
s32 func_0026D2D0(void *, s32);                 /* extern */

typedef struct {
    /* 0x0000 */ u8 pad0[0x6A88];
    /* 0x6A88 */ s32 pending;
} Unk00235FF4;

s32 func_00235FF4(Unk00235FF4 *arg0, s32 arg1) {
    s32 temp_s0;

    temp_s0 = arg0->pending;
    if (temp_s0 != 0) {
        func_0026D2D0(&arg0->pending, temp_s0);
        func_0026D288(arg1 + 0x40, temp_s0);
    }
    return temp_s0;
}
