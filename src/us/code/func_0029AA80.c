/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_27.c, func_802BA6B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
void osViSetMode(OSViMode *modep) {
    register u32 saveMask = __osDisableInt();
    __osViNext->modep = modep;
    __osViNext->state = 1;
    __osViNext->control = __osViNext->modep->comRegs.ctrl;
    __osRestoreInt(saveMask);
}
