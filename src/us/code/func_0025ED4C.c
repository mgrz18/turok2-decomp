#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x0 */ f32 alpha;
    /* 0x4 */ f32 alphaMax;
} FillAlpha;

/* Draw override: fill mode, then the fill colour right after it. */
extern s32 D_800AB8B0;
#define sFillColor ((CVector3 *) (&D_800AB8B0 + 1))
extern FillAlpha D_800AB8C0;

void func_0021D928(CGameObjectInstance *obj, CAI *ai, s32 arg2);

void func_0025ED4C(CGameObjectInstance *obj, CAI *ai, s32 arg2) {
    f32 fade = 1.0f;
    CVector3 *color;
    FillAlpha *alpha;

    if (ai->m_ModeTime < 15.0f) {
        color = sFillColor;
        color->z = 0;
        color->x = 255.0f;
        color->y = 255.0f;
        fade = 1.0f - ai->m_ModeTime * (1.0f / 15.0f);
    } else {
        color = sFillColor;
        color->x = color->y = color->z = 0.0f;
        if (ai->m_ModeTime < 90.0f) {
            fade = (ai->m_ModeTime - 15.0f) * (1.0f / 75.0f);
        }
    }
    D_800AB8B0 = 3;
    alpha = &D_800AB8C0;
    alpha->alphaMax = 255.0f;
    alpha->alpha = fade * 210.0f;
    func_0021D928(obj, ai, arg2);
    D_800AB8B0 = 0;
}
