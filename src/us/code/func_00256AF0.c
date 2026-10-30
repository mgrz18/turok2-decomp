/* Cut fixed in versions/cuts.us.txt: func_00256AF0 really is 0x256AF0-0x256B34 (0x44 bytes, like its twin func_00256AAC): splat cut its low-nibble return off as func_00256B24 (whose C in src/ is a fake for that tail). */
#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x52C];
    /* 0x52C */ u8 counters[1]; /* two 4-bit counters per byte */
} CounterOwner;

/* func_00256A8C, inlined. */
static __inline__ u8 getCounter(CounterOwner *owner, s32 index, s32 high) {
    if (high != 0) {
        return owner->counters[index] >> 4;
    }
    return owner->counters[index] & 0xF;
}

/* Drops one nibble of a packed counter byte and returns that nibble. */
u8 func_00256AF0(CounterOwner *owner, s32 index, s32 high) {
    if (high != 0) {
        owner->counters[index] -= 0x10;
    } else {
        owner->counters[index] -= 1;
    }
    return getCounter(owner, index, high);
}
