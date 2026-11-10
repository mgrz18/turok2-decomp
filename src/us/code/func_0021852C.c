#include "common.h"

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used + missing_args): the caller (func_00218620) passes
 * (track, time, 0, 0) and uses the f32 result; on MIPS a0/a1 pass straight through. */
extern f32 AnimCurve_Evaluate(void *t, f32 x);
f32 func_0021852C(void *t, f32 x, s32 a2, s32 a3) {
    return AnimCurve_Evaluate(t, x);
}
#else
s32 AnimCurve_Evaluate();                            /* extern */

void func_0021852C(void) {
    AnimCurve_Evaluate();
}
#endif
