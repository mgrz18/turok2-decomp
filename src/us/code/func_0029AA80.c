/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_27.c, func_802BA6B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern __OSViContext_func_802BA6B0_de *D_800B7AC4;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32);
void func_0029AA80(OSViMode *modep) {
    register u32 saveMask = func_0029DFF0();
    D_800B7AC4->modep = modep;
    D_800B7AC4->state = 1;
    D_800B7AC4->control = D_800B7AC4->modep->comRegs.w0;
    func_0029E010(saveMask);
}
