#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/engine.h"
#include "structs/light.h"
#include "structs/model.h"
#include "protos/game.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
} Matrix18;

/* the draw callback's CDrawArgs (structs/model.h): m_hModel at 0x0C is pointer-wide on the host; the
 * ROM's own view read it as an s32 and m_nModel at 0x18 as "flag" (round 10 y2: the host crashed in
 * Render_DrawModel on the truncated model handle) */
typedef CDrawArgs DrawInfo;

extern void ScriptAnimator_Apply(ScriptAnimator *light);
extern void ScreenTint_UpdateMode(void);

void BlindOne_Draw(CGameObjectInstance *actor, CAI *ai, DrawInfo *info) {
    ScriptAnimator_Apply(&gBlindOne.faders[ai->m_BlindOne.m_nFader]);
    ScreenTint_UpdateMode();
    switch (actor->m_nTypeFlag) {
        case 0x450:
            if (ai->m_Mode == 9) {
                info->m_nModel = 1;
            }
            break;
        case 0x455:
            if (ai->m_Mode == 6) {
                info->m_nModel = 1;
            }
            break;
    }
    Render_DrawModel(info->m_hModel, actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0, info->m_nModel, actor->m_AI.m_nTexture);
    gBlendState = 0;
}
