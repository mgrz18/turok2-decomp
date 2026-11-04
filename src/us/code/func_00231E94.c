#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x118];
    /* 0x118 */ s32 sound;
    /* 0x11C */ u8 pad11C[0x1B5 - 0x11C];
    /* 0x1B5 */ u8 registered;
} Actor;

s32 func_00228B4C(s32 *, s32);              /* extern */
s32 func_002327F4();                            /* extern */
s32 func_00234E48(s32 *, void *);           /* extern */

extern s32 D_8010FF34;
extern s32 D_801102B8;

void func_00231E94(Actor *arg0) {
    s32 temp_a1;

    if (arg0->registered != 0) {
        func_002327F4();
        func_00234E48(&D_801102B8, arg0);
    }
    temp_a1 = arg0->sound;
    if (temp_a1 != 0) {
        func_00228B4C(&D_8010FF34, temp_a1);
        arg0->sound = 0;
    }
}
