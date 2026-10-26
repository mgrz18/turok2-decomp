#include "common.h"

/* Render blend state shared with func_0022782C / func_00236324. */
typedef struct {
    /* 0x00 */ s32 mode;
    /* 0x04 */ f32 color[3];
} BlendState;

extern BlendState D_800AB8B0;
extern f32 D_800AB8C0[2]; /* [0] fade, [1] alpha */

static __inline__ void clamp255(f32 *v) {
    if (*v < 0.0f) {
        *v = 0.0f;
    } else if (*v > 255.0f) {
        *v = 255.0f;
    }
}

void func_00235898(void) {
    f32 alpha;
    f32 fade;

    clamp255(&D_800AB8B0.color[0]);
    clamp255(&D_800AB8B0.color[1]);
    clamp255(&D_800AB8B0.color[2]);
    clamp255(&D_800AB8C0[0]);
    clamp255(&D_800AB8C0[1]);

    fade = D_800AB8C0[0];
    alpha = D_800AB8C0[1];
    if (alpha == 255.0f && fade == 0.0f) {
        D_800AB8B0.mode = 0;
    } else if (alpha != 255.0f && fade == 0.0f) {
        D_800AB8B0.mode = 1;
        D_800AB8C0[0] = D_800AB8C0[1];
    } else if (alpha == 255.0f && fade != 0.0f) {
        D_800AB8B0.mode = 3;
    } else if (alpha != 255.0f && fade != 0.0f) {
        D_800AB8B0.mode = 6;
    }
}
