/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444530.c, func_00444530.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern s32 func_0041648C(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern CPlayerOptions D_8011AB18;
s32 func_0041BF08(MenuItem *a, Menu *b) {
    CPlayerOptions *p = &D_8011AB18;
    if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
    p->m_ControlType = func_0041648C((s32)b, p->m_ControlType, 1, 0, 1, 1);
    return 0;
}
