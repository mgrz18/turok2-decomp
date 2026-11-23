/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044465C.c, func_0044465C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sOptionsMenuText_ControlSideRight[];
extern char sOptionsMenuText_ControlSideLeft[];
s32 OptionsMenu_ControlSideText_2(MenuItem *a, Menu *b) {
CPlayerOptions *p = &gEngineApp_m_Settings.defaultSlot;
if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
 if (p->m_ControlSide != 0) { if (p->m_ControlSide == 1) a->text = sOptionsMenuText_ControlSideLeft; } else a->text = sOptionsMenuText_ControlSideRight;
 return 0; }
