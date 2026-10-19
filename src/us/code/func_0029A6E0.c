/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_21.c, func_802BA310_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *func_0029A6E0();
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32);
extern Field_void_4 *D_800B7AC0;
void *func_0029A6E0(void) {
    void *result = func_0029DFF0();
    void *saved = D_800B7AC0->value;
    func_0029E010(result);
    return saved;
}
