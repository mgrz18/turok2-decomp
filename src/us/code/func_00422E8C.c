/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0040ACD4.c, func_0040ACD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 D_801309C0;
extern char D_800B4BB0[];
extern char D_800B4BB4[];
s32 PakMessage_UpdateOk(MenuItem *item) {
    /* Rage Wars sends 5, 6, 9, 10, 12 and 15 to the second text; Turok 2's
     * jump table (jtbl_800C1630, 0-15) sends only 8 there. The other case
     * values are not in the table; 0 and 15 bound it. */
    switch (D_801309C0) {
    default:
    case 0:
    case 5:
    case 6:
    case 9:
    case 10:
    case 12:
    case 15:
        item->text = D_800B4BB0;
        break;
    case 8:
        item->text = D_800B4BB4;
        break;
    }
    return 0;
}
