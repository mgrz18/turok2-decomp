/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_8.c, func_802457E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 Cinema_PathDone(void);
s32 Cinema_PathDone(void) {
    extern func_802457D0_S1 *gCinemaPlayerP;
    if (gCinemaPlayerP->unk38 != 0) {
        if (gCinemaPlayerP->unk1C > gCinemaPlayerP->unk34) {
            return 1;
        }
        return 0;
    }
    return 1;
}
