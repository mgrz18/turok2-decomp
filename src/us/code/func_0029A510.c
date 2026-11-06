#include "common.h"

/* The record whose status word at 4 gets the RSP status bits. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 status;
} SpState;

s32 __osSpGetStatus(void);

s32 osSpTaskYielded(SpState *arg0) {
    s32 temp_a0;
    u32 temp_v0;

    temp_v0 = __osSpGetStatus();
    temp_a0 = temp_v0 >> 8;
    temp_a0 &= 1;
    if (temp_v0 & 0x80) {
        arg0->status = (arg0->status | temp_a0) & ~2;
    }
    return temp_a0;
}
