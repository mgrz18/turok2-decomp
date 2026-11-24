/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_13.c, func_8023C8D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * globals/pager.h (round 29, part r2: Rage Wars' Shape_typemap_110 is the pager's first 0x20
 * bytes, PagerStats). */

#include "common.h"
#include "structs/pager.h"
#include "globals/pager.h"

/* Copies the pager's counters out. */
void VM_GetStats(PagerStats *out) {
    *out = *(PagerStats *)&gPager;
}
