#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x1B8];
    /* 0x1B8 */ s32 loop;               /* looping sound handle */
} SoundObject;

s32 func_00275D34(s32, s32);                    /* extern */
s32 func_00275F7C();                                /* extern */

void func_00232B40(SoundObject *arg0) {
    if (arg0->loop != 0) {
        func_00275D34(func_00275F7C(), arg0->loop);
        arg0->loop = 0;
    }
}
