#include "common.h"

/* The cinema state: a path playing between two times. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 time;
    /* 0x20 */ u8 pad20[0x30 - 0x20];
    /* 0x30 */ f32 start;
    /* 0x34 */ f32 end;
    /* 0x38 */ s32 active;
} Cinema;

extern Cinema *D_800C1BB0;

s32 Cinema_PathPlaying(void) {
    f32 temp_f1;
    s32 var_a0;

    var_a0 = 0;
    if (D_800C1BB0->active != 0) {
        temp_f1 = D_800C1BB0->time;
        if ((D_800C1BB0->start <= temp_f1) && (temp_f1 <= D_800C1BB0->end)) {
            var_a0 = 1;
        }
    }
    return var_a0;
}
