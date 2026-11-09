/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043768C_eu_padding_1277BC_14.c, func_8044605C_eu.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

s32 PlayerSetupMenu_Play(void *first, void *second, void *third) {
    MenuPool_OpenChildMenu(third, second, &gRumbleNoticeMenuDesc);
    return 1;
}
