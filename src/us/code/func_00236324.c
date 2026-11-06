#include "common.h"
#include "structs/vector.h"
#include "structs/light.h"
#include "structs/boss.h"

void ScriptAnimator_Apply(ScriptAnimator *arg0) {
    CVector3 *temp_v0;
    CVector3 *temp_v0_2;

    if (arg0->alpha != 0.0f) {
        temp_v0 = (CVector3 *) gBlendState_m_Color;
        gBlendState_m_Color[0] = (f32) arg0->rgb[0];
        temp_v0->y = (f32) arg0->rgb[1];
        temp_v0->z = (f32) arg0->rgb[2];
    } else {
        temp_v0_2 = (CVector3 *) gBlendState_m_Color;
        gBlendState_m_Color[0] = 0.0f;
        temp_v0_2->y = 0.0f;
        temp_v0_2->z = 0.0f;
    }
    gBlendState_m_Color[3] = (f32) arg0->alpha;
    gBlendState_m_Color[4] = (f32) arg0->value;
}
