/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00293F38.c, func_00293F38.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"
#include "globals/engine.h"

extern f32 D_800A9DFC;
u32 GetMemSize();
s32 CEngineApp__TimedModeSwitch(CEngineApp *g, f32 x, s32 a, s32 b);
void EngineMode01_Legal_Retrace(CEngineApp *g) {
    s32 mode;
    u32 r = GetMemSize();
    mode = 4;
    if (r > 0x400000) {
        if (gShowExpansionPakScreen != 0) {
            mode = 3;
        }
    }
    if (g->m_Settings.modeTimer > D_800A9DFC) {
        gLegalScreenNoSkip = 0;
    }
    if (gLegalScreenNoSkip != 0) {
        if (CEngineApp__TimedModeSwitch(g, 12.0f, mode, -1)) {
            gLegalScreenNoSkip = 0;
        }
    } else {
        if (CEngineApp__TimedModeSwitch(g, 12.0f, mode, mode)) {
            gLegalScreenNoSkip = 0;
        }
    }
}
