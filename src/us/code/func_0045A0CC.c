/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_14.c, func_80409744_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 gPakMenuPort;
extern void PakMenu_PickDefaultPort_2(void);
void Pak_ProbeSlot_2(s32);
s32 Pak_GetStatus_2(s32);
void PakMenu_PickDefaultPort_2(void) {
    s32 var_s0;
    s32 var_s1;
    s32 unavailable;
    if (gPakMenuPort == -1) {
        var_s0 = 0;
        var_s1 = 0;
    unavailable = -2;
loop_2:
        Pak_ProbeSlot_2(var_s0);
        if (Pak_GetStatus_2(var_s0) == unavailable) {
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
