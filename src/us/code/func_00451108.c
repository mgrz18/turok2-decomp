/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043E520.c, func_0043E520.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern u8 sPlayerSetupMenuText_ControlExpert[];
extern u8 sPlayerSetupMenuText_ControlArcade[];
s32 PlayerSetupMenu_ControlTypeLabel_2(MenuItem *a, Menu *m) {
    switch (m->player->m_pOptions->m_ControlType) {
    case 0:
        a->text = (char *)sPlayerSetupMenuText_ControlExpert;
        break;
    case 1:
        a->text = (char *)sPlayerSetupMenuText_ControlArcade;
        break;
    }
    return 0;
}
