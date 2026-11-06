/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_9.c, func_802645D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int Controller_IsPakWatched(int arg0);
extern int gPakChanged[];
s32 Controller_PakChanged(s32 arg0) {
    if (Controller_IsPakWatched(arg0) == 0) {
        return 0;
    }
    return gPakChanged[arg0];
}
int Controller_IsPakWatched(int arg0);
