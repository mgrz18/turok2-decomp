/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_27.c, func_802BA6B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern __OSViContext *D_800B7AC4;
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
void osViSetMode(OSViMode *modep) {
    register u32 saveMask = __osDisableInt();
    D_800B7AC4->modep = modep;
    D_800B7AC4->state = 1;
    D_800B7AC4->control = D_800B7AC4->modep->comRegs.ctrl;
    __osRestoreInt(saveMask);
}
