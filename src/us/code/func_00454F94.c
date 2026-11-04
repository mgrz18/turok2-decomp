/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044465C.c, func_0044465C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern CPlayerOptions D_8011AB18;
extern char D_800B3F18[];
extern char D_800B3F00[];
s32 func_00454F94(MenuItem *a, Menu *b) {
CPlayerOptions *p = &D_8011AB18;
if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
 if (p->m_ControlSide != 0) { if (p->m_ControlSide == 1) a->text = D_800B3F00; } else a->text = D_800B3F18;
 return 0; }
