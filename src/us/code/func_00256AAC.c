#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x52C];
    /* 0x52C */ u8 counters[1]; /* two 4-bit counters per byte */
} CounterOwner;

/* PickupCounter_Get, inlined. */
static __inline__ u8 getCounter(CounterOwner *owner, s32 index, s32 high) {
    if (high != 0) {
        return owner->counters[index] >> 4;
    }
    return owner->counters[index] & 0xF;
}

/* Bumps one nibble of a packed counter byte and returns that nibble. */
u8 PickupCounter_Inc(CounterOwner *owner, s32 index, s32 high) {
    if (high != 0) {
        owner->counters[index] += 0x10;
    } else {
        owner->counters[index] += 1;
    }
    return getCounter(owner, index, high);
}
