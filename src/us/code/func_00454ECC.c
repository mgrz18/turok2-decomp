/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444594.c, func_00444594.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern CPlayerOptions D_8011AB18;
extern char D_800B3EB8[];
extern char D_800B3ED0[];
s32 OptionsMenu_ControlTypeText_2(MenuItem *a, Menu *b) {
CPlayerOptions *p = &D_8011AB18;
if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
 switch (p->m_ControlType) { case 0: a->text = D_800B3EB8; break; case 1: a->text = D_800B3ED0; break; }
 return 0; }
