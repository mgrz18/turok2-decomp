#include "common.h"
#include "structs/vector.h"
#include "structs/light.h"
#include "structs/boss.h"

void ScriptAnimator_Apply(ScriptAnimator *arg0) {
    CVector3 *temp_v0;
    CVector3 *temp_v0_2;

    if (arg0->alpha != 0.0f) {
        temp_v0 = &D_800AB8B0.m_Color;
        D_800AB8B0.m_Color.x = (f32) arg0->rgb[0];
        temp_v0->y = (f32) arg0->rgb[1];
        temp_v0->z = (f32) arg0->rgb[2];
    } else {
        temp_v0_2 = &D_800AB8B0.m_Color;
        D_800AB8B0.m_Color.x = 0.0f;
        temp_v0_2->y = 0.0f;
        temp_v0_2->z = 0.0f;
    }
    D_800AB8C0[0] = (f32) arg0->alpha;
    D_800AB8C0[1] = (f32) arg0->value;
}
