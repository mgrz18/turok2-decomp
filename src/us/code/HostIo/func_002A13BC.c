/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/IO/80200568_2.CPP, func_802005DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

#include "common.h"
#include "globals/hostio.h"
extern void HostIo_Delay(int arg0);
void HostIo_Delay(s32 arg0) {
    for (sHostIoDelayCounter = 0; sHostIoDelayCounter < arg0; sHostIoDelayCounter++) {
    }
}
