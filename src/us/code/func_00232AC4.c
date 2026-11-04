#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 state;
    /* 0x0C */ s32 sound;
} SoundHandle;

/* An object with a looping sound attached. */
typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ f32 pos[3];
    /* 0x010 */ u8 pad10[0x1B8 - 0x10];
    /* 0x1B8 */ SoundHandle *loop;
} SoundObject;

s32 func_00275C6C(s32, s32, s32, s32, s32);
s32 func_00232B40(SoundObject *);                      /* extern */
s32 func_00275F7C();                                /* extern */

/* Starts (or restarts) the looping sound on the object. */
void func_00232AC4(SoundObject *arg0, s32 arg1) {
    s32 var_s1;
    f32 *temp_a2;
    SoundHandle *temp_v1;
    SoundObject *var_s0;

    var_s0 = arg0;
    temp_v1 = var_s0->loop;
    var_s1 = arg1;
    if (temp_v1 != NULL) {
        if ((temp_v1->sound != var_s1) || (temp_v1->state == -1)) {
            func_00232B40(var_s0);
            goto block_4;
        }
    } else {
block_4:
        temp_a2 = var_s0->pos;
        var_s0->loop = (SoundHandle *) func_00275C6C(func_00275F7C(), var_s1, (s32) temp_a2, (s32) temp_a2, -1);
    }
}
