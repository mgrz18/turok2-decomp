/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444D58.c, func_00444D58.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern CPlayerOptions gEngineApp_m_Settings_defaultSlot;
extern char D_800B40B0[];
extern char D_800B40C8[];
s32 OptionsMenu_WeaponSelectText_2(MenuItem *a, Menu *b) {
 CPlayer *q = b->player;
 CPlayerOptions *p;
 if (q) p = q->m_pOptions; else p = &gEngineApp_m_Settings_defaultSlot;
 if (p->m_WeaponSelect == 1) a->text = D_800B40B0; else a->text = D_800B40C8;
 return 0;
}
