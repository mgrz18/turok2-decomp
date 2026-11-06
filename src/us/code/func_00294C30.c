#include "common.h"

/* Only the field read here: a rate, in units per million. */
typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 rate;
} RateSource;

extern f32 D_800AA4CC;

/* arg1 scaled by the rate, rounded down to a multiple of 16. */
s32 _timeToSamplesNoRound(RateSource *arg0, s32 arg1) {
    return (s32) (((f32) arg1 * (f32) arg0->rate * 0.000001f) + D_800AA4CC) & ~0xF;
}
