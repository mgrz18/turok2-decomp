#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/engine.h"
#include "structs/light.h"
#include "protos/game.h"
#include "structs/twin_blindone.h"


#include "structs/model.h"

void ScriptAnimator_Apply(ScriptAnimator *light);
void ScreenTint_UpdateMode(void);

void BlindOne_Draw_2(CGameObjectInstance *actor, CAI *ai, CDrawArgs *info) {
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
