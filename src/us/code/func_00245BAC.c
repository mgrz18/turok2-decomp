/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80213ED4_de_11.c, func_80216808_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/engine.h"

extern void Actor_TurnBy(void *arg0, f32 arg1);
void AI_TurnClamped(void *arg0, s32 arg1, f32 arg2, f32 arg3) {
    f32 f0;
    f32 f1;
    f32 f2;
    f32 f3;
    f0 = arg2;
    f1 = arg3;
    f3 = f1;
    if (f0 < f1 || (f0 = -f0, f1 < f0)) {
        f1 = f0;
    }
    f1 = f1 * gFrameIncrement;
    f2 = f1;
    if (f1 < 0.0f) {
        f2 = -f1;
    }
    if (f3 < 0.0f) {
        if (-f3 < f2) {
            goto clamp;
        }
    } else if (f3 < f2) {
clamp:
        f1 = f3;
    }
    Actor_TurnBy(arg0, f1);
}
