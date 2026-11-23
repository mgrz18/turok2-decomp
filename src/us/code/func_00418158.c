/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043E520.c, func_0043E520.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern u8 D_800B4210[];
extern u8 D_800B4220[];
s32 PlayerSetupMenu_ControlTypeLabel(MenuItem *a, Menu *m) {
    switch (m->player->m_pOptions->m_ControlType) {
    case 0:
        a->text = (char *)D_800B4210;
        break;
    case 1:
        a->text = (char *)D_800B4220;
        break;
    }
    return 0;
}
