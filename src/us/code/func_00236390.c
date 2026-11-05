#include "common.h"
#include "structs/boss.h"

/* Draw fade/fog accumulators: D_800AB8B0 the mode, then four floats; the
 * last of them is also D_800AB8C0[0]. */
typedef struct {
    /* 0x00 */ s32 mode;
    /* 0x04 */ f32 add[4];
} FadeState;

extern FadeState D_800AB8B0;
extern f32 D_800AB8C0[2];

void func_00236390(ScriptAnimator *arg0) {
    f32 *temp_v0;

    if (arg0->alpha != 0.0f) {
        temp_v0 = D_800AB8B0.add;
        D_800AB8B0.add[0] = (f32) (D_800AB8B0.add[0] + arg0->rgb[0]);
        temp_v0[1] = (f32) (temp_v0[1] + arg0->rgb[1]);
        temp_v0[2] = (f32) (temp_v0[2] + arg0->rgb[2]);
        temp_v0[3] = (f32) (temp_v0[3] + arg0->alpha);
    }
    D_800AB8C0[1] = (f32) (D_800AB8C0[1] + arg0->value);
}
