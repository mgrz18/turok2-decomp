/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040ACD4.c, func_0040ACD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern char sPakMessageText_Error[];
extern char sPakMessageText_Important[];
s32 PakMessage_UpdateHeading_2(MenuItem *item) {
    switch (gPakMessageKind) {
    default:
    case 0:
        item->text = sPakMessageText_Error;
        break;
    case 5:
    case 6:
    case 9:
    case 10:
    case 12:
    case 15:
        item->text = sPakMessageText_Important;
        break;
    }
    return 0;
}
