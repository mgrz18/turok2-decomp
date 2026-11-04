#include "common.h"

void *func_002017D4(s32, s32);                  /* extern */
s32 func_002097AC(s32, f32, void *);            /* extern */
extern s32 D_800B6D18;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ f32 value;
} Param;

void func_0020B4F0(s32 *arg0, f32 arg1) {
    f32 temp_f0;
    s32 temp_s0;
    Param *temp_v0;

    temp_s0 = *arg0;
    temp_v0 = func_002017D4(temp_s0, 3);
    if (temp_v0->id != D_800B6D18) {
        temp_f0 = temp_v0->value + arg1;
        temp_v0->value = temp_f0;
        func_002097AC(temp_s0, temp_f0, temp_v0);
    }
}
