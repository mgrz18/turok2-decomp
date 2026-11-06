/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_13.c, func_8023C8D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void VM_GetStats(struct Shape_typemap_110 *arg0);
extern struct Shape_typemap_110 gPager;
void VM_GetStats(struct Shape_typemap_110 *arg0) {
    *arg0 = gPager;
}
