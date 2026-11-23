/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444D58.c, func_00444D58.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_text.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 OptionsMenu_WeaponSelectText_2(MenuItem *a, Menu *b) {
 CPlayer *q = b->player;
 CPlayerOptions *p;
 if (q) p = q->m_pOptions; else p = &gEngineApp_m_Settings.defaultSlot;
 if (p->m_WeaponSelect == 1) a->text = sOptionsMenuText_WeaponSelectQuick; else a->text = sOptionsMenuText_WeaponSelectNorm;
 return 0;
}
