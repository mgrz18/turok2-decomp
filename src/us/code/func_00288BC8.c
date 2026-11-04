#include "common.h"
#include "structs/game.h"

s32 func_0028D0E0();                                /* extern */
extern f32 D_800A9DE0;

/* The frame/game state block passed to the per-mode handlers (the
 * handler table D_800B6D78/D_800B6D7C has 0xC-byte rows indexed by mode). */

void func_00288BC8(CEngineApp *arg0, s32 arg1) {
    s32 temp_v1;

    temp_v1 = arg0->m_Settings.mode;
    arg0->m_Settings.fadeMode = 1;
    arg0->m_Settings.fade = 0.0f;
    arg0->m_Settings.mode = 0x11;
    arg0->m_Settings.nextMode = arg1;
    arg0->m_Settings.prevMode = temp_v1;
    if (func_0028D0E0() != 0) {
        arg0->m_Settings.fade = D_800A9DE0;
    }
}
