#include "common.h"

/* A swoosh trail (see Trail_Draw). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 model[0x34 - 0x14];
    /* 0x34 */ s32 texture;
} Trail;

s32 func_00207914(void *, s32);                 /* extern */
s32 func_002316A0(s32, void *);                 /* extern */

void func_0023685C(s32 arg0, Trail *arg1, s32 arg2) {
    s32 temp_a1;
    void *temp_s0;

    temp_a1 = arg1->texture;
    if (temp_a1 != -1) {
        temp_s0 = arg1->model;
        func_00207914(temp_s0, temp_a1);
        if (arg2 != 0) {
            func_002316A0(arg2, temp_s0);
        }
    }
}
