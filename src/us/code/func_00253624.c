#include "common.h"

/* A record with a state word at 0xB4 (0 and 3 are left alone). */
typedef struct {
    /* 0x00 */ u8 pad0[0xB4];
    /* 0xB4 */ s32 state;
} StateRecord;

void func_00253624(StateRecord *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->state;
    if ((temp_v0 != 0) && (temp_v0 != 3)) {
        arg0->state = 3;
    }
}
