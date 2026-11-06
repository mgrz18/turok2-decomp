/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_14.c, func_80409744_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"

extern void PakMenu_PickDefaultPort(void);
void Pak_ProbeSlot(s32);
s32 Pak_GetStatus(s32);
void PakMenu_PickDefaultPort(void) {
    s32 var_s0;
    s32 var_s1;
    s32 unavailable;
    if (gPakMenuPort == -1) {
        var_s0 = 0;
        var_s1 = 0;
    unavailable = -2;
loop_2:
        Pak_ProbeSlot(var_s0);
        if (Pak_GetStatus(var_s0) == unavailable) {
            var_s0 += 1;
            if (var_s0 >= 4) {
                var_s0 = 0;
            }
            var_s1 += 1;
            if (var_s1 < 4) {
                goto loop_2;
            }
        }
        if (var_s1 == 4) {
            var_s0 = -1;
        }
        gPakMenuPort = var_s0;
    }
}
