/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_8.c, func_8040A748_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void PakConfirmMenu_InitNoSpace(void);
extern int gPakConfirmKind;
extern s32 sPakConfirmDone;
extern s32 gPakConfirmKind;
void PakConfirmMenu_InitNoSpace(void) {
    gPakConfirmKind = 1;
    sPakConfirmDone = 0;
}
