/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_21.c, func_802BA310_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
void *osViGetCurrentFramebuffer(void) {
    void *result = __osDisableInt();
    void *saved = __osViCurr->framep;
    __osRestoreInt(result);
    return saved;
}
