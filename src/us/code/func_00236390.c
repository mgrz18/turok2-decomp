#include "common.h"
#include "structs/light.h"
#include "structs/boss.h"

void func_00236390(ScriptAnimator *arg0) {
    f32 *temp_v0;

    if (arg0->alpha != 0.0f) {
        temp_v0 = &D_800AB8B0.m_Color.x; /* [3] runs into D_800AB8C0[0] */
        D_800AB8B0.m_Color.x = (f32) (D_800AB8B0.m_Color.x + arg0->rgb[0]);
        temp_v0[1] = (f32) (temp_v0[1] + arg0->rgb[1]);
        temp_v0[2] = (f32) (temp_v0[2] + arg0->rgb[2]);
        temp_v0[3] = (f32) (temp_v0[3] + arg0->alpha);
    }
    D_800AB8C0[1] = (f32) (D_800AB8C0[1] + arg0->value);
}
