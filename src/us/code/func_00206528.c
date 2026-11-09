/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_14.c, func_80254D00_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cache.h"
#include "protos/cache.h"

void CCache__ClearCurrentDisplayList(void) {
    s32 *p = &gCache_currentDlist;
    CCache__ClearDisplayListLocks(0, *p);
    gCache_dlistValid[*p] = 0;
}
