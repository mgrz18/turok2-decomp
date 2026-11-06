/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de.c, func_80267260_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void CGameRegion__FloodClearFlag4(void *arg0);
extern void CGameRegion__FloodSetFlag4(void *arg0);
extern void GameObject_TraceMove(void *arg0, Triple input, Triple *output,
                          s32 *value, s32 unused0, s32 unused1);
void func_00237FAC(void *arg0, Triple input, s32 arg4) {
    Triple output;
    s32 value;
    GameObject_TraceMove(arg0, input, &output, &value, 0, 0);
    if (arg4 != 0) {
        CGameRegion__FloodSetFlag4((void *)value);
    } else {
        CGameRegion__FloodClearFlag4((void *)value);
    }
}
