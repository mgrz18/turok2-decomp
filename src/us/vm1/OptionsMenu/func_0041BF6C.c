/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444594.c, func_00444594.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_text.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 OptionsMenu_ControlTypeText(MenuItem *a, Menu *b) {
CPlayerOptions *p = &gEngineApp_m_Settings.defaultSlot;
if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
 switch (p->m_ControlType) { case 0: a->text = sOptionsMenuText_ControlTypeExpert; break; case 1: a->text = sOptionsMenuText_ControlTypeArcade; break; }
 return 0; }
