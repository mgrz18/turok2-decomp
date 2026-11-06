#include "common.h"
#include "structs/light.h"
#include "structs/boss.h"

void func_00236390(ScriptAnimator *arg0) {
    f32 *temp_v0;

    if (arg0->alpha != 0.0f) {
        temp_v0 = &gBlendState_m_Color[0]; /* [3] runs into gBlendState_m_Color[3] */
        gBlendState_m_Color[0] = (f32) (gBlendState_m_Color[0] + arg0->rgb[0]);
        temp_v0[1] = (f32) (temp_v0[1] + arg0->rgb[1]);
        temp_v0[2] = (f32) (temp_v0[2] + arg0->rgb[2]);
        temp_v0[3] = (f32) (temp_v0[3] + arg0->alpha);
    }
    gBlendState_m_Color[4] = (f32) (gBlendState_m_Color[4] + arg0->value);
}
