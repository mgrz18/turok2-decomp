#include "common.h"

/* A playing sound. */
typedef struct {
    /* 0x00 */ s32 handle;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 state;
    /* 0x0C */ u8 padC[0x1E - 0xC];
    /* 0x1E */ s16 unk1E;
} Sound;

s32 func_002718F4(s32);                             /* extern */
s32 func_00293E60(s32, s16);                    /* extern */
s32 func_00293EC0(s32);                         /* extern */

void func_00277024(Sound *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = arg0->state;
    if ((temp_v0 != 2) && (temp_v0 != 0)) {
        arg0->state = 2;
        temp_v0_2 = func_002718F4(arg0->handle);
        func_00293E60(temp_v0_2, arg0->unk1E);
        func_00293EC0(temp_v0_2);
    }
}
