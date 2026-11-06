/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80291054_10.CPP, func_80293904_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/game.h"

extern s32 gMergedController_buttonsSingle;
extern f32 D_800A9DE4;

s32 CEngineApp__TimedModeSwitch(CEngineApp *ctx, f32 value, s32 normalNext, s32 forcedNext) {
    s32 *flag = &gMergedController_buttonsSingle;

    if (*flag != 0 && forcedNext != -1) {
        ctx->m_Settings.fadeMode = 2;
        ctx->m_Settings.nextMode = forcedNext;
        *flag = 0;
        return 1;
    }
    if (value * D_800A9DE4 < ctx->m_Settings.modeTimer) {
        ctx->m_Settings.fadeMode = 2;
        ctx->m_Settings.nextMode = normalNext;
        return 1;
    }
    return 0;
}
