#include "common.h"

/* What this reads and writes. */
typedef struct {
    /* 0x00 */ s32 handle;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 state;
    /* 0x0C */ u8 padC[0x1E - 0xC];
    /* 0x1E */ s16 sound;
} SoundSlot;

void func_002714B0(s32);                            /* extern */
void func_00271514(s32);                            /* extern */
s32 func_002718F4(s32);                             /* extern */
void func_00293E60(s32, s16);                       /* extern */
void func_00293EC0(s32);                            /* extern */

void func_00276DFC(SoundSlot *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    func_002714B0(arg0->handle);
    temp_v0 = arg0->state;
    if ((temp_v0 != 2) && (temp_v0 != 0)) {
        arg0->state = 2;
        temp_v0_2 = func_002718F4(arg0->handle);
        func_00293E60(temp_v0_2, arg0->sound);
        func_00293EC0(temp_v0_2);
    }
    func_00271514(arg0->handle);
}
