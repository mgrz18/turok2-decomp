#include "common.h"
#include "structs/game_object.h"
#include "structs/light.h"

typedef struct {
    /* 0x0 */ f32 alpha;
    /* 0x4 */ f32 alphaMax;
} FillAlpha;


void CGameObjectInstance__DrawModelImmediate(CGameObjectInstance *obj, CAI *ai, sptr arg2);

void EnemyAI_DrawWithBlend(CGameObjectInstance *obj, CAI *ai, s32 arg2) {
    f32 fade = 1.0f;
    CVector3 *color;
    FillAlpha *alpha;

    if (ai->m_ModeTime < 15.0f) {
        color = (CVector3 *) gBlendState_m_Color;
        color->z = 0;
        color->x = 255.0f;
        color->y = 255.0f;
        fade = 1.0f - ai->m_ModeTime * (1.0f / 15.0f);
    } else {
        color = (CVector3 *) gBlendState_m_Color;
        color->x = color->y = color->z = 0.0f;
        if (ai->m_ModeTime < 90.0f) {
            fade = (ai->m_ModeTime - 15.0f) * (1.0f / 75.0f);
        }
    }
    gBlendState = 3;
    alpha = (FillAlpha *) &gBlendState_m_Color[3];
    alpha->alphaMax = 255.0f;
    alpha->alpha = fade * 210.0f;
    CGameObjectInstance__DrawModelImmediate(obj, ai, arg2);
    gBlendState = 0;
}
