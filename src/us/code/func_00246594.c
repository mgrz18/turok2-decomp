#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;
typedef struct { f32 x, y, z; } V3f;

typedef struct { s16 *base; s16 *p; } S;

s16 func_00246594(S *arg0) {
    s16 v = *arg0->p++;

    if (*arg0->p == -1) {
        arg0->p = arg0->base;
    }
    return v;
}
