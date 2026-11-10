/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_9.c, func_802645D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

#ifdef TARGET_PORT
extern u8 Controller_IsPakWatched(int arg0); /* ABI: returns u8 (the host leaves the upper bits of the return register undefined) */
#else
extern int Controller_IsPakWatched(int arg0);
#endif
s32 Controller_PakChanged(s32 arg0) {
    if (Controller_IsPakWatched(arg0) == 0) {
        return 0;
    }
    return gPakChanged[arg0];
}
#ifdef TARGET_PORT
u8 Controller_IsPakWatched(int arg0); /* ABI: returns u8 (the host leaves the upper bits of the return register undefined) */
#else
int Controller_IsPakWatched(int arg0);
#endif
