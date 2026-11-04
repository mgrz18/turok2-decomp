/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00293378.c, func_00293378.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"

extern f32 D_800A9DD8;
extern f32 D_800A9DDC;
extern s32 D_800C2030;
extern s32 D_800C2034;
void func_002883E0(CEngineApp *g, u8 *col, s32 a, s32 b, s32 c, s32 d);
void func_00288B1C(CEngineApp *g) {
    u8 col[4];
    f32 t;
    s32 v;
    col[0] = 0;
    col[1] = 0;
    col[2] = 0;
    t = g->m_Settings.fade;
    if (t < 0.0f) goto c255;
    if (D_800A9DD8 < t) { v = 0; goto done; }
    if (t < 0.0f) { c255: v = 255; } else { v = ~(s32)(t * D_800A9DDC); }
    done:;
    col[3] = v;
    func_002883E0(g, col, 0, 0, D_800C2030, D_800C2034);
}
