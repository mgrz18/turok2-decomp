/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00293378.c, func_00293378.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"
#include "globals/render.h"

void CEngineApp__DrawFilledRect(CEngineApp *g, u8 *col, s32 a, s32 b, s32 c, s32 d);
void CEngineApp__DrawFade(CEngineApp *g) {
    u8 col[4];
    f32 t;
    s32 v;
    col[0] = 0;
    col[1] = 0;
    col[2] = 0;
    t = g->m_Settings.fade;
    if (t < 0.0f) goto c255;
    if (1.0f < t) { v = 0; goto done; }
    if (t < 0.0f) { c255: v = 255; } else { v = ~(s32)(t * 255.0f); }
    done:;
    col[3] = v;
    CEngineApp__DrawFilledRect(g, col, 0, 0, gScreenWidth, gScreenHeight);
}
