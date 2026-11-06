#include "common.h"
#include "structs/light.h"

static __inline__ void clamp255(f32 *v) {
    if (*v < 0.0f) {
        *v = 0.0f;
    } else if (*v > 255.0f) {
        *v = 255.0f;
    }
}

void ScreenTint_UpdateMode(void) {
    f32 alpha;
    f32 fade;

    clamp255(&D_800AB8B0.m_Color.x);
    clamp255(&D_800AB8B0.m_Color.y);
    clamp255(&D_800AB8B0.m_Color.z);
    clamp255(&D_800AB8C0[0]);
    clamp255(&D_800AB8C0[1]);

    fade = D_800AB8C0[0];
    alpha = D_800AB8C0[1];
    if (alpha == 255.0f && fade == 0.0f) {
        D_800AB8B0.m_Mode = 0;
    } else if (alpha != 255.0f && fade == 0.0f) {
        D_800AB8B0.m_Mode = 1;
        D_800AB8C0[0] = D_800AB8C0[1];
    } else if (alpha == 255.0f && fade != 0.0f) {
        D_800AB8B0.m_Mode = 3;
    } else if (alpha != 255.0f && fade != 0.0f) {
        D_800AB8B0.m_Mode = 6;
    }
}
