#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x52C];
    /* 0x52C */ u8 counters[1]; /* two 4-bit counters per byte */
} CounterOwner;

u8 func_00256A8C(CounterOwner *owner, s32 index, s32 high) {
    if (high != 0) {
        return owner->counters[index] >> 4;
    }
    return owner->counters[index] & 0xF;
}
