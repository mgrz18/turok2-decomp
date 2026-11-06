/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_22.c, func_8040ABA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern void PakMessage_InitRumblePak(void);
void PakMessage_InitRumblePak(void) {
    gPakMessageKind = 9;
    sPakMessageRumbleB = 0;
}
