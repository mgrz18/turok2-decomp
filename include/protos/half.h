/* code/HalfToFloat: the packed 16-bit floats of the sound envelopes, particles and events
 * (round 29, part r2). The definitions take and give the forms below. */

#ifndef PROTOS_HALF_H
#define PROTOS_HALF_H

#include "common.h"

u16 FloatToHalf(f32 x);
f32 HalfToFloat(s32 h);

#endif
