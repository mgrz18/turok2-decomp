#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x0 */ f32 alpha;
    /* 0x4 */ f32 alphaMax;
} FillAlpha;

/* Draw override: fill mode, then the fill colour right after it (gBlendState,
 * CBlendState in structs/light.h: through the struct this does not match). */
extern s32 gBlendState;
#define sFillColor ((CVector3 *) (&gBlendState + 1))
extern FillAlpha gBlendFade;

void CGameObjectInstance__DrawModelImmediate(CGameObjectInstance *obj, CAI *ai, s32 arg2);

void EnemyAI_DrawWithBlend(CGameObjectInstance *obj, CAI *ai, s32 arg2) {
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
    gBlendState = 3;
    alpha = &gBlendFade;
    alpha->alphaMax = 255.0f;
    alpha->alpha = fade * 210.0f;
    CGameObjectInstance__DrawModelImmediate(obj, ai, arg2);
    gBlendState = 0;
}
