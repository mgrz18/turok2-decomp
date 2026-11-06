/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_21.c, func_802BA310_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *osViGetCurrentFramebuffer();
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
extern Field_void_4 *D_800B7AC0;
void *osViGetCurrentFramebuffer(void) {
    void *result = __osDisableInt();
    void *saved = D_800B7AC0->value;
    __osRestoreInt(result);
    return saved;
}
