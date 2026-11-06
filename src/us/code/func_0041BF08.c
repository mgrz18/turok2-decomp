/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444530.c, func_00444530.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 Menu_StepValue(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 OptionsMenu_StepControlType(MenuItem *a, Menu *b) {
    CPlayerOptions *p = &gEngineApp_m_Settings.defaultSlot;
    if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
    p->m_ControlType = Menu_StepValue((s32)b, p->m_ControlType, 1, 0, 1, 1);
    return 0;
}
