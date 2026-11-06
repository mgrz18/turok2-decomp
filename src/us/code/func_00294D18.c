#include "common.h"


typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 rate;
} Unk00294D18;

s32 _timeToSamples(Unk00294D18 *arg0, s32 arg1) {
    return (s32) (((f32) arg1 * (f32) arg0->rate * 0.000001f) + 0.5f);
}
