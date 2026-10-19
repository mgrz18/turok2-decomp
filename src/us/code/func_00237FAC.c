/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de.c, func_80267260_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0026C590(void *arg0);
extern void func_0026C55C(void *arg0);
extern void func_002688C8(void *arg0, Triple input, Triple *output,
                          s32 *value, s32 unused0, s32 unused1);
void func_00237FAC(void *arg0, Triple input, s32 arg4) {
    Triple output;
    s32 value;
    func_002688C8(arg0, input, &output, &value, 0, 0);
    if (arg4 != 0) {
        func_0026C55C((void *)value);
    } else {
        func_0026C590((void *)value);
    }
}
